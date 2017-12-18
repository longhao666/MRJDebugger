#include "driveroncan.h"
#include <time.h>
#include <sstream>

using std::vector;
using std::string;
using std::endl;

DriverOnCan::DriverOnCan() :
    Driver(),
    m_hCan(NULL),
    hEvent(NULL),
    outfile(static_cast<FILE*>(NULL)),
    isLog(false)
{
    allJoint.clear();
    allGripper.clear();
    hMutex = CreateMutex(NULL,FALSE,NULL);
}

DriverOnCan::~DriverOnCan()
{

}

// 由于C++的限制，线程的入口函数不能是类的成员函数
unsigned int __stdcall ReadThread(void* arg) {
    if (arg == NULL) {
        return 1; // 有错误
    }
    DriverOnCan & DOC = *(static_cast<DriverOnCan *>(arg));
    DOC.thread_read(); // 真正想在线程中执行的函数
    return 0;
}

bool DriverOnCan::ThreadStart()
{
    // 当一个等待线程被释放时，自动重置为无信号状态，初始是无信号状态
    hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    // 设置CAN盒，启动事件功能
    TPCANStatus result = CAN_SetValue(m_hCan, PCAN_RECEIVE_EVENT, &hEvent, sizeof(hEvent));
    // An error occurred, get a text describing the error
    if (result != PCAN_ERROR_OK) {
        char strMsg[256];
        CAN_GetErrorText(result, 0, strMsg);
        errorTxt = strMsg;
        return false;
    }

    // 开启新线程 //    WaitForSingleObject(handle, 5); // 等待5ms
    HANDLE handle = (HANDLE)_beginthreadex(NULL,
                                           0,
                                           ReadThread,
                                           static_cast<void*>(this),
                                           0,
                                           NULL);
    if ( handle == 0 ) {
        errorTxt = "Failed to create thread !";
        return false;
    } else {
        errorTxt = "thread_read init success";
    }
    // 初始化CAN线上的设备
    searchIDandInitMemoryTable();

    return true;
}

void *DriverOnCan::thread_read() {
    while (true) {
        DWORD dReturn = WaitForSingleObject(hEvent,INFINITE);
        // 事件触发
        if ( WAIT_OBJECT_0 == dReturn) {
            bool isOK = true;
            while (isOK) { // 要一直读到队列空
                isOK = ReceiveMsg();
            }
        }
    }
}

TPCANHandle NodeConvertToChannel(const char *szDeviceNode) {
    int len = static_cast<int>(strlen(szDeviceNode));
    if (len < 5 || len > 6) {
        return 0;
    }
    if (szDeviceNode[0] == 'p' && szDeviceNode[1] == 'c' && szDeviceNode[2] == 'a' && szDeviceNode[3] == 'n') {
        if (len == 5 && szDeviceNode[4] >= '0' && szDeviceNode[4] <= '7') {
            return PCAN_USBBUS1 + static_cast<TPCANHandle>(szDeviceNode[4] - '0');
        }
        if (len == 5 && szDeviceNode[4] == '8') {
            return PCAN_USBBUS9;
        }
        if (len == 6 && szDeviceNode[4] == '1' && szDeviceNode[5] >= '0' && szDeviceNode[5] <= '5') {
            return PCAN_USBBUS10 + static_cast<TPCANHandle>(szDeviceNode[5] - '0');
        }
    }
    else {
        return 0;
    }
    return 0;
}


/**
 * @brief DriverOnCan::BusInit
 * @param deviceName
 * @return
 * 函数功能:初始化总线
 * 参数:设备号名称
 *
 */
bool DriverOnCan::BusInit(const char* deviceName)
{
    // 获取PCAN要初始化的设备号
    m_hCan = NodeConvertToChannel(deviceName);
    // 调用PCAN API初始化PCAN
    TPCANStatus result = CAN_Initialize(m_hCan, PCAN_BAUD_1M);
    // An error occurred, get a text describing the error
    if (result != PCAN_ERROR_OK) {
        char strMsg[256];
        CAN_GetErrorText(result, 0, strMsg);
        errorTxt = strMsg;
        return false;
    } else {
        errorTxt = "PCAN-USB was initialized";
        return true;
    }
}

bool DriverOnCan::BusClose(const char* deviceName)
{
    TPCANStatus result;
    char strMsg[256];

    // 获取PCAN的设备号
    TPCANHandle m_hCan = NodeConvertToChannel(deviceName);
    if (m_hCan != 0) {
        result = CAN_Uninitialize(m_hCan);
        m_hCan = 0;
    } else {
        errorTxt = "An error occurred in Can exit.";
        return false;
    }

    // An error occurred, get a text describing the error
    if (result != PCAN_ERROR_OK) {
        CAN_GetErrorText(result, 0, strMsg);
        errorTxt = strMsg;
        return false;
    } else {
        errorTxt = "PCAN-USB was released";
        return true;
    }
}

void DriverOnCan::outLog(TPCANMsg txmsg, bool isSend)
{
    if (!outfile) {
        errorTxt = "An error occurred while writing the log!";
        return;
    }
    outfile << std::hex << txmsg.ID << "\t";
    if (isSend) {
        outfile << "Tx" << "\t";
    } else {
        outfile << "Rx" << "\t";
    }
    outfile << static_cast<int>(txmsg.LEN);
    for (int i = 0; i < static_cast<int>(txmsg.LEN); i++) {
         outfile << std::hex
             << "\t" << static_cast<int>(txmsg.DATA[i]);
    }
    outfile << endl;
}

// 便于平台移植
void DriverOnCan::delayMs(uint32_t millisecond)
{
    Sleep(millisecond);
}

bool DriverOnCan::SendMsg(uint16_t ID,
                          uint8_t commandType,
                          uint8_t addr,
                          uint8_t *data,
                          uint8_t dataLength)
{
    TPCANMsg txmsg;
    // clear the txmsg
    memset(&txmsg, 0, sizeof(TPCANMsg));

    // 若发送的是写指令
    if (commandType == CMDTYPE_WR || commandType == CMDTYPE_WR_NR) {
        if (dataLength > 6) {
            errorTxt = "ERROR: arm_sendmsg error, the dataLength must <= 6 !";
            return false;
        }
        // Construct the txmsg
        // 1 the STID
        txmsg.ID = ID;
        txmsg.MSGTYPE = PCAN_MESSAGE_STANDARD;

        // 2 the DLC/4 is the dataLength + 2
        txmsg.LEN = dataLength + 2;

        // 3 the data 1 Byte is the commandType
        txmsg.DATA[0] = commandType;
        txmsg.DATA[1] = addr;
        memcpy(&txmsg.DATA[2], data, dataLength);

        // 处理日志
        if (isLog) {
            outLog(txmsg, true);
        }
        //Send the TX message
        WaitForSingleObject(hMutex,INFINITE);
        TPCANStatus result = CAN_Write(m_hCan, &txmsg);//CAN_Write()这一步不能保证资源不会竞争
        ReleaseMutex(hMutex);
        // An error occurred, get a text describing the error
        if (result != PCAN_ERROR_OK) {
            char strMsg[256];
            CAN_GetErrorText(result, 0, strMsg);
            if (commandType == CMDTYPE_WR) {
                errorTxt = "CMDTYPE_WR ERROR: write 1 returned 1\n";
            } else {
                errorTxt = "CMDTYPE_WR_NR ERROR: write returned 1\n";
            }
            errorTxt += strMsg;
            return false;
        } else {
            return true;
        }
    } else if (commandType == CMDTYPE_RD) {
        // Construct the txmsg
        // 1 the STID
        txmsg.ID = ID;
        txmsg.MSGTYPE = PCAN_MESSAGE_STANDARD;
        // 2 the DLC/4 is 03 when read commandType
        txmsg.LEN = 0x03;
        // 3 the data 1 Byte is the commandType
        txmsg.DATA[0] = commandType;
        txmsg.DATA[1] = addr;
        txmsg.DATA[2] = static_cast<uint8_t>(dataLength);

        //Send the TX message
        if (isLog) {
            outLog(txmsg, true);
        }
        WaitForSingleObject(hMutex,INFINITE);
        TPCANStatus result = CAN_Write(m_hCan, &txmsg);//CAN_Write()这一步不能保证资源不会竞争
        ReleaseMutex(hMutex);

        // An error occurred, get a text describing the error
        if (result != PCAN_ERROR_OK) {
            char strMsg[256];
            CAN_GetErrorText(result, 0, strMsg);
            errorTxt = "ERROR: CMDTYPE_RD sendmsg returned 1\n";
            errorTxt += strMsg;
            return false;
        } else {
            return true;
        }
    }
    else if (commandType == CMDTYPE_WR_REG) {
        errorTxt = "Error: CMDTYPE_WR_REG";
        return false;
    }
    else if (commandType == CMDTYPE_SCP) {
        errorTxt = "Error: CMDTYPE_SCP";
        return false;
    }
    else if (commandType == CMDTYPE_RD_HP) {
        errorTxt = "Error: CMDTYPE_RD_HP";
        return false;
    } else {
        errorTxt = "commandType error, the commandType type is out of define!";
        return false;
    }
}

bool DriverOnCan::ReceiveMsg()
{
    TPCANMsg rxmsg;
    memset(&rxmsg, 0, sizeof(TPCANMsg));

    TPCANTimestamp timestamp;
    TPCANStatus result;

    /* Read the RX message */
    result = CAN_Read(m_hCan, &rxmsg, &timestamp);
    if (result != PCAN_ERROR_OK) {
        //线程里一直在读，不应该一直返回信息显示
        return false;
    }
    if(isLog) {
        outLog(rxmsg, false);
    }
    if (rxmsg.ID == 0) {
        errorTxt = "ERROR: rxmsg.ID == 0";
        return false;
    }
    uint32_t theID = (uint32_t)(rxmsg.ID - (uint32_t)0x100); // 应答包格式设定所致
    // 只在读指令和示波器数据返回时，改写MemoryControlTable
    if (rxmsg.DATA[CAN_CMD] != CMDTYPE_RD && rxmsg.DATA[CAN_CMD] != CMDTYPE_SCP) {
        // 有返回的写
        return false;
    }
    if (theID >= SEARCH_MAX_ID) {
        errorTxt = "ERROR: theID has wrong range";
        return false;
    }
    uint8_t index = rxmsg.DATA[CAN_INDEX];
//            cout << "Receive Msg index:" << index << endl;
//            if (index < 0 || index >= CMDLENGTH) {
//                cout << "ERROR: index has wrong range\n";
//                return false;
//            }// 读超过1字的数据，这样的index检查不够用
    // rxmsg.LEN - 2 含义是 数据域字节长度 减去CMD和Index
    // 除2 含义是 MemoryControlTable是以 字 为单位，而数据域以字节为单位，需要转换
    bool flagFindID = false;
    // 先给关节添加
    vector<Joint>::iterator i = allJoint.begin();
    while (i != allJoint.end()) {
        if (i->ID == theID) {
            break;
        }
        ++i;
    }
    Joint * j = NULL;
    if (i != allJoint.end()) {
        j = &*i;
    } else {
        j = NULL;
    }
    if (j != NULL) {
        flagFindID = true;
        for (BYTE i = 0; i < (rxmsg.LEN - 2) / 2; i++) {
            //数据是低位在前
            uint16_t value = (uint16_t)(rxmsg.DATA[CAN_DATA + i * 2] +
                    (rxmsg.DATA[CAN_DATA + i * 2 + 1] << 8));
            j->MemoryControlTable[index + i] = value;
            // 示波器数据同时影响SYSCUR SPD POS
            switch(index) {
                case SCP_MEACUR_L: {
                    j->MemoryControlTable[SYS_CURRENT_L + i] = value;
                    break;
                }
                case SCP_MEACUR_H: {
                    j->MemoryControlTable[SYS_CURRENT_H + i] = value;
                    break;
                }
                case SCP_MEASPD_L: {
                    j->MemoryControlTable[SYS_SPEED_L + i] = value;
                    break;
                }
                case SCP_MEASPD_H: {
                    j->MemoryControlTable[SYS_SPEED_H + i] = value;
                    break;
                }
                case SCP_MEAPOS_L: {
                    j->MemoryControlTable[SYS_POSITION_L + i] = value;
                    break;
                }
                case SCP_MEAPOS_H: {
                    j->MemoryControlTable[SYS_POSITION_H + i] = value;
                    break;
                }
            }
        }
    }
    //再给手爪添加
    vector<Gripper>::iterator ig = allGripper.begin();
    while (ig != allGripper.end()) {
        if (ig->ID == theID) {
            break;
        }
        ++ig;
    }
    Gripper * g = NULL;
    if (ig != allGripper.end()) {
        g = &*ig;
    } else {
        g = NULL;
    }
    if (g != NULL) {
        flagFindID = true;
        for (BYTE i = 0; i < (rxmsg.LEN - 2) / 2; i++) {
            //数据是低位在前
            uint16_t value = (uint16_t)(rxmsg.DATA[CAN_DATA + i * 2] +
                    (rxmsg.DATA[CAN_DATA + i * 2 + 1] << 8));
            g->MemoryControlTable[index + i] = value;
        }
    }
    if (flagFindID == false) {
        allJoint.push_back(Joint(theID,this)); // 初始时默认都是Joint
    }
    return true;
}

bool DriverOnCan::GetValueInTable(uint16_t ID, uint8_t addr, uint16_t &data) {
    if (ID > SEARCH_MAX_ID) {
        errorTxt = "ID is over the max id range";
        return false;
    }
    if (addr > CMDLENGTH) {
        errorTxt = "addr  is over the max cmd range";
        return false;
    }

    // 在关节和手爪中寻找
    bool flagFindID = false;
    for (vector<Joint>::iterator iter = allJoint.begin(); iter != allJoint.end(); ++iter) {
        if (iter->ID == ID) {
            data = iter->MemoryControlTable[addr];
            flagFindID = true;
            break;
        }
    }
    for (vector<Gripper>::iterator iter = allGripper.begin(); iter != allGripper.end(); ++iter) {
        if (iter->ID == ID) {
            data = iter->MemoryControlTable[addr];
            flagFindID = true;
            break;
        }
    }
    if (flagFindID == false) {
        string errorTxt = "ID: ";
        std::stringstream ss;
        ss << ID;
        errorTxt += ss.str();
        errorTxt += " is not used";
        return false;
    }
    return true;
}

/**
 * @brief DriverOnCan::searchIDandInitMemoryTable
 * 寻找ID并且初始化内存表
 */
void DriverOnCan::searchIDandInitMemoryTable() {
    // 初始化容器
    allJoint.clear();
    allGripper.clear();
    // 全部合法的ID都查询是否已连接
    for (uint16_t i = 1; i < SEARCH_MAX_ID; i++) {
        SendMsg(i, CMDTYPE_RD, SYS_ID, NULL, 0x02);
    }
    // 等待成功消息返回
    delayMs(20); // 休眠10 ms读不到ID，20可以，15可以，13概率性不行，14概率性不行
    // 已连接设备初始化其型号
    for (vector<Joint>::iterator iter = allJoint.begin(); iter != allJoint.end(); ++iter) {
        iter->updateModelType();
    }
    delayMs(500);// 要确保初始化完成
    // 区分关节和手爪，驱动器型号：0x10-M14，0x20-M17,0x21-M17E,0x30-M20，0x80-Gripper
    for (vector<Joint>::iterator iter = allJoint.begin(); iter != allJoint.end(); ++iter) {
        if (iter->getModelType() == 0x80) {
            allGripper.push_back(Gripper(iter->ID, this)); // 添加到手爪
        }
    }
    // 从关节中删除已添加到手爪的ID
    for (vector<Gripper>::iterator iter = allGripper.begin(); iter != allGripper.end(); ++iter) {
        for (vector<Joint>::iterator iter2 = allJoint.begin(); iter2 != allJoint.end(); ++iter2) {
            if (iter2->ID == iter->ID) {
                allJoint.erase(iter2);
                break; // 结束当前循环
            }
        }
    }
    // 初始化关节的内存表
    for (vector<Joint>::iterator iter = allJoint.begin(); iter != allJoint.end(); ++iter) {
        iter->MemoryTableInit();
    }
    // 初始化手爪的内存表
    for (vector<Gripper>::iterator iter = allGripper.begin(); iter != allGripper.end(); ++iter) {
        iter->MemoryTableInit();
    }
    // 等待初始化内存表完成，休眠150 ms读不全，ID，180可以，170概率性不行
    delayMs(200); // 由log查看，400 ms，1500仍有返回，1800概率性返回，2000可以
}

bool DriverOnCan::updateAllID() {
    try {
        searchIDandInitMemoryTable();
    } catch (std::exception) {
        errorTxt = "updateAllID is failed!";
        return false;
    }
    errorTxt = "No error";
    return true;
}

void DriverOnCan::SetLog(bool isLogging)
{
    isLog = isLogging;
    if (isLogging) {
        outfile.open("CAN.log", std::ofstream::app);
        time_t t = time(NULL);
        char tmp[64];
        strftime( tmp, sizeof(tmp), "%Y-%m-%d %X",localtime(&t) );
        outfile << endl;
        outfile << "------------------------------------" << endl;
        outfile << "Start recording at " << tmp << endl;
        outfile << "------------------------------------" << endl;
        outfile << "ID Rx/Tx Length Data" << endl;
        outfile << "------------------------------------" << endl;
    } else {
        outfile.close();
    }
}
