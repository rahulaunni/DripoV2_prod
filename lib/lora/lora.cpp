#include "lora.h"
static RadioEvents_t RadioEvents;
SX1276MB1xAS Radio( NULL );
uint16_t Buffer_Size = BUFFER_SIZE;
uint8_t rxBuffer[BUFFER_SIZE];

Lora::Lora(){}
//destructor function
Lora::~Lora(){}
//lora initialization
void Lora::init(){
    // Initialize Radio driver
    RadioEvents.TxDone = OnTxDone;
    RadioEvents.RxDone = OnRxDone;
    RadioEvents.RxError = OnRxError;
    RadioEvents.TxTimeout = OnTxTimeout;
    RadioEvents.RxTimeout = OnRxTimeout;
    Radio.Init( &RadioEvents );
      // verify the connection with the board
    while( Radio.Read( REG_VERSION ) == 0x00  )
    {
        debug( "Radio could not be detected!\n\r", NULL );
        wait( 1 );
    }

    debug_if( ( DEBUG_MESSAGE & ( Radio.DetectBoardType( ) == SX1276MB1LAS ) ), "\n\r > Board Type: SX1276MB1LAS < \n\r" );
    debug_if( ( DEBUG_MESSAGE & ( Radio.DetectBoardType( ) == SX1276MB1MAS ) ), "\n\r > Board Type: SX1276MB1MAS < \n\r" );

    Radio.SetChannel( RF_FREQUENCY ); 

#if USE_MODEM_LORA == 1

    debug_if( LORA_FHSS_ENABLED, "\n\n\r             > LORA FHSS Mode < \n\n\r" );
    debug_if( !LORA_FHSS_ENABLED, "\n\n\r             > LORA Mode < \n\n\r" );

    //LoRa TX Configuration
    Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                         LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                         LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                         LORA_CRC_ENABLED, LORA_FHSS_ENABLED, LORA_NB_SYMB_HOP,
                         LORA_IQ_INVERSION_ON, 2000 );
    //LoRa RX Configuration
    Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                         LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                         LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON, 0,
                         LORA_CRC_ENABLED, LORA_FHSS_ENABLED, LORA_NB_SYMB_HOP,
                         LORA_IQ_INVERSION_ON, true );
#elif USE_MODEM_FSK == 1

    debug("\n\n\r              > FSK Mode < \n\n\r" );
    Radio.SetTxConfig( MODEM_FSK, TX_OUTPUT_POWER, FSK_FDEV, 0,
                         FSK_DATARATE, 0,
                         FSK_PREAMBLE_LENGTH, FSK_FIX_LENGTH_PAYLOAD_ON,
                         FSK_CRC_ENABLED, 0, 0, 0, 2000 );

    Radio.SetRxConfig( MODEM_FSK, FSK_BANDWIDTH, FSK_DATARATE,
                         0, FSK_AFC_BANDWIDTH, FSK_PREAMBLE_LENGTH,
                         0, FSK_FIX_LENGTH_PAYLOAD_ON, 0, FSK_CRC_ENABLED,
                         0, 0, false, true );

#else

#error "Please define a modem in the compiler options."

#endif
}

//function to send message 
bool Lora::send(bool flag,char topic[],char message[]){
    if(flag == true){
        strcpy( ( char* )Buffer, ( char* )topic );
        strcat(( char* )Buffer,message);
        Radio.Send( Buffer, BufferSize );
        printf("sending message -- %s\n\r",Buffer);
        return false;
    }
    else{
        return false;
    }

}

//function to change lora to Rx mode
int Lora::rxMode(int RxTimeout){
    Radio.Rx(RxTimeout);
    return 0;
}

//function to retrieve recieved message
unsigned char * Lora::getMessage(){
    printf("retrieved message -- %s\n\r",rxBuffer);
    return rxBuffer;
}



void OnTxDone( void )
{
    Radio.Sleep( );
    printf("> OnTxDone\n\r" );
}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    Radio.Sleep( );
    Buffer_Size = size;
    memcpy( rxBuffer, payload, Buffer_Size );
    printf("recieved message -- %s\n\r",rxBuffer);
    printf("> OnRxDone\n\r" );
}

void OnTxTimeout( void )
{
    Radio.Sleep( );
    printf("> OnTxTimeout\n\r" );
}

void OnRxTimeout( void )
{
    Radio.Sleep( );
    printf("> OnRxTimeout\n\r" );
}

void OnRxError( void )
{
    Radio.Sleep( );
    printf("> OnRxError\n\r" );
}
