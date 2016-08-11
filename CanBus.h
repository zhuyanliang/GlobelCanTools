#ifndef CANBUS_H
#define CANBUS_H
#include "DataType.h"
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

#include "ControlCAN.h"

#ifdef __cplusplus
}
#endif

class CanBus
{
private:

    static CanBus* cInstance;
    CanBus();

    int m_DevType;
    int m_DevIndex;
    int m_CanIndex;
    int m_Baudrate;
    PVCI_INIT_CONFIG m_pInitConfig;
    bool m_isOpen;  //b=false - close
                    //b=true - open
    bool m_isStart;

public:

    static CanBus* getInstance()
    {
        if( cInstance == NULL )
        {
            cInstance = new CanBus();
        }

        return cInstance;

    }

    void setDevType(int devType){m_DevType = devType;}
    void setDevIndex(int devIndex){m_DevIndex = devIndex;}
    void setBaudrate(int baudrate){m_Baudrate = baudrate;}
    void setCanIndex(int canIndex){m_CanIndex = canIndex;}
    inline void setInitConfig(PVCI_INIT_CONFIG pInitConfig){
        memcpy(m_pInitConfig,pInitConfig,sizeof(VCI_INIT_CONFIG));
    }
    void setOpenorClose(bool b){m_isOpen = b;}
    void setStartorStop(bool b){m_isStart = b;}

    int getDevType(void){return m_DevType;}
    int getDevIndex(void){return m_DevIndex;}
    int getCanIndex(void){return m_CanIndex;}
    int getBaudrate(void){return m_Baudrate;}
    inline bool getIsOpen(){return m_isOpen;}
    inline bool getIsStart(){return m_isStart;}
    inline PVCI_INIT_CONFIG getInitConfig(void){return m_pInitConfig;}

public:

    DWORD CanOpen(DWORD devType,DWORD devIndex,int reserved);
    DWORD CanClose(DWORD devType,DWORD devIndex);
    DWORD CanInit(DWORD devType,DWORD devIndex,DWORD canIndex,
                  PVCI_INIT_CONFIG pInitConfig);
    DWORD CanReadBoardInfo(DWORD devType, DWORD devIndex, PVCI_BOARD_INFO pInfo);
    DWORD CanReadErrInfo(DWORD devType, DWORD devIndex, DWORD canIndex,
                         PVCI_ERR_INFO pErrInfo);
    DWORD CanReadCanStatus(DWORD devType, DWORD devIndex,
                         DWORD canIndex, PVCI_CAN_STATUS pCANStatus);
    DWORD CanGetReference(DWORD devType, DWORD devIndex, DWORD canIndex,
                          DWORD RefType,PVOID pData);
    DWORD CanSetReference(DWORD devType, DWORD devIndex,
                          DWORD canIndex, DWORD refType,PVOID pData);
    ULONG CanGetReceiveNum(DWORD devType, DWORD devIndex, DWORD canIndex);
    DWORD CanClearBuffer(DWORD devType, DWORD devIndex, DWORD canIndex);
    DWORD CanStartCAN(DWORD devType, DWORD devIndex, DWORD canIndex);
    DWORD CanResetCAN(DWORD devType, DWORD devIndex, DWORD canIndex);
    ULONG CanTransmit(DWORD devType, DWORD devIndex, DWORD canIndex,
                      PVCI_CAN_OBJ pSend,ULONG len);
    ULONG CanReceive(DWORD devType, DWORD devIndex, DWORD canIndex,
                     PVCI_CAN_OBJ pReceive,ULONG len, INT waitTime);
};

#endif // CANBUS_H
