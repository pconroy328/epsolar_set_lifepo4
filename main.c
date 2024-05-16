/* 
 * File:   main.c
 * Author: pconroy
 *
 * Created on November 8, 2023, 10:09 PM
 */

#include <stdlib.h>
#include <ctype.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <log4c.h>
#include <libepsolar.h>
#include <libmqttrv.h>
#include <cjson/cJSON.h>
#include <getopt.h>




static  int     sleepSeconds = 60;                  // How often to send out SCC data packets
static  char    *brokerHost = "mqttrv.local";       // default address of our MQTT broker
static  int     passedInBrokerHost = FALSE;         // TRUE if they passed it in via the command line
static  int     loggingLevel = 5;

static  char    *topTopic = "SCC";                  // MQTT top level topic
static  char    publishTopic[ 1024 ];               // published data will be on "<topTopic>/<controlleID>/STATUS"
static  char    subscriptionTopic[ 1024 ];          // subscribe to <"<topTopic>/<controlleID>/COMMAND"

static  int     synchClocks  = TRUE;
static  int     controllerID = 1;
static  char    *devicePortName = NULL;


extern char    *getCurrentDateTime (void);




int main(int argc, char** argv) 
{
    const char *devPort = "/dev/ttyXRUSB0";
    
    Logger_Initialize( "/tmp/epsolar_statusbits_mqtt.log", 5 );
    Logger_LogWarning( "Version: %s\n", "v1.0"  );

    if (!epsolarModbusConnect( devPort, 1 )) {
        Logger_LogFatal( "Unable to open %s to connect to the solar charge controller\n", devPort );
        return( EXIT_FAILURE );
    }
    
    
    Logger_LogInfo( "Setting parameters for Lithium\n" );
    Logger_LogInfo( "Before\n" );
    
    Logger_LogInfo( "eps_getHighVoltageDisconnect: %f\n", eps_getHighVoltageDisconnect());
    Logger_LogInfo( "eps_getChargingLimitVoltage: %f\n" , eps_getChargingLimitVoltage());
    Logger_LogInfo( "eps_getOverVoltageReconnect: %f\n" , eps_getOverVoltageReconnect());
    Logger_LogInfo( "eps_getEqualizationVoltage: %f\n" , eps_getEqualizationVoltage() );
    Logger_LogInfo( "eps_getBoostingVoltage: %f\n" , eps_getBoostingVoltage());
    Logger_LogInfo( "eps_getFloatingVoltage: %f\n" , eps_getFloatingVoltage());
    Logger_LogInfo( "eps_getBoostReconnectVoltage: %f\n" ,eps_getBoostReconnectVoltage());
    Logger_LogInfo( "eps_getLowVoltageReconnectVoltage: %f\n" , eps_getLowVoltageReconnectVoltage());
    Logger_LogInfo( "eps_getLowVoltageWarningRecoverVoltage: %f\n" , eps_getUnderVoltageWarningRecoverVoltage());
    Logger_LogInfo( "eps_getLowVoltageWarningVoltage: %f\n" , eps_getUnderVoltageWarningVoltage());
    Logger_LogInfo( "eps_getLowVoltageDisconnectVoltage: %f\n" , eps_getLowVoltageDisconnectVoltage());
    Logger_LogInfo( "eps_getDischargingLimitVoltage: %f\n" , eps_getDischargingLimitVoltage());
    Logger_LogInfo( "eps_getBoostDuration: %d\n" , eps_getBoostDuration());
    Logger_LogInfo( "eps_getEqualizeDuration: %d\n" , eps_getEqualizeDuration());
    Logger_LogInfo( "eps_getTemperatureCompensationCoefficient: %d\n" ,eps_getTemperatureCompensationCoefficient());
    
    Logger_LogInfo( "Setting\n ");
    
    eps_setHighVoltageDisconnect( 14.4 );
    eps_setChargingLimitVoltage( 14.2 );
    eps_setOverVoltageReconnect( 14.2 );
    
    eps_setEqualizationVoltage( 14.1 );
    eps_setBoostingVoltage( 14.1 );
    eps_setFloatingVoltage( 13.7 );
    eps_setBoostReconnectVoltage( 13.3 );
    
    eps_setLowVoltageReconnectVoltage( 11.5 );
    eps_setLowVoltageWarningRecoverVoltage( 11.5 );
    eps_setLowVoltageWarningVoltage( 11.5 );
    eps_setLowVoltageDisconnectVoltage( 11.5 );
    eps_setDischargingLimitVoltage( 11.5 );
    
    eps_setBoostDuration( 120 );
    eps_setEqualizeDuration( 120 );
    
    eps_setTemperatureCompensationCoefficient( 2 );

    Logger_LogInfo( "After\n" );
    
    Logger_LogInfo( "eps_getHighVoltageDisconnect: %f\n", eps_getHighVoltageDisconnect());
    Logger_LogInfo( "eps_getChargingLimitVoltage: %f\n" , eps_getChargingLimitVoltage());
    Logger_LogInfo( "eps_getOverVoltageReconnect: %f\n" , eps_getOverVoltageReconnect());
    Logger_LogInfo( "eps_getEqualizationVoltage: %f\n" , eps_getEqualizationVoltage() );
    Logger_LogInfo( "eps_getBoostingVoltage: %f\n" , eps_getBoostingVoltage());
    Logger_LogInfo( "eps_getFloatingVoltage: %f\n" , eps_getFloatingVoltage());
    Logger_LogInfo( "eps_getBoostReconnectVoltage: %f\n" ,eps_getBoostReconnectVoltage());
    Logger_LogInfo( "eps_getLowVoltageReconnectVoltage: %f\n" , eps_getLowVoltageReconnectVoltage());
    Logger_LogInfo( "eps_getLowVoltageWarningRecoverVoltage: %f\n" , eps_getUnderVoltageWarningRecoverVoltage());
    Logger_LogInfo( "eps_getLowVoltageWarningVoltage: %f\n" , eps_getUnderVoltageWarningVoltage());
    Logger_LogInfo( "eps_getLowVoltageDisconnectVoltage: %f\n" , eps_getLowVoltageDisconnectVoltage());
    Logger_LogInfo( "eps_getDischargingLimitVoltage: %f\n" , eps_getDischargingLimitVoltage());
    Logger_LogInfo( "eps_getBoostDuration: %d\n" , eps_getBoostDuration());
    Logger_LogInfo( "eps_getEqualizeDuration: %d\n" , eps_getEqualizeDuration());
    Logger_LogInfo( "eps_getTemperatureCompensationCoefficient: %d\n" ,eps_getTemperatureCompensationCoefficient());

    Logger_LogInfo( "Disconnecting from charge controller\n" );
    epsolarModbusDisconnect();
   
    Logger_Terminate();
    return (EXIT_SUCCESS);
}

// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
static  char    currentDateTimeBuffer[ 80 ];
char    *getCurrentDateTime (void)
{
    //
    // Something quick and dirty... Fix this later - thread safe
    time_t  current_time;
    struct  tm  *tmPtr;
 
    memset( currentDateTimeBuffer, '\0', sizeof currentDateTimeBuffer );
    
    /* Obtain current time as seconds elapsed since the Epoch. */
    current_time = time( NULL );
    if (current_time > 0) {
        /* Convert to local time format. */
        tmPtr = localtime( &current_time );
 
        if (tmPtr != NULL) {
            strftime( currentDateTimeBuffer,
                    sizeof currentDateTimeBuffer,
                    "%FT%T%z",                           // ISO 8601 Format
                    tmPtr );
        }
    }
    
    return &currentDateTimeBuffer[ 0 ];
}

void showHelp() 
{}

// -----------------------------------------------------------------------------
static
void    parseCommandLine (int argc, char *argv[])
{
    //
    //  Options
    //  -h  <string>    MQTT host to connect to
    //  -t  <string>    MQTT top level topic
    //  -s  N           sleep between sends <seconds>
    //  -i  <string>    give this controller an identifier (defaults to LS1024B_1)
    //  -p  <string>    open this /dev/port to talk to contoller (defaults to /dev/ttyUSB0
    char    c;
    
    while (((c = getopt( argc, argv, "h:t:s:i:p:v:j" )) != -1) && (c != 255)) {
        switch (c) {
            case 'h':   brokerHost = optarg;
                        passedInBrokerHost = TRUE;
                        break;
                        
            case 's':   sleepSeconds = atoi( optarg );  break;
            case 't':   topTopic = optarg;              break;
            case 'i':   controllerID = atoi( optarg );  break;
            case 'p':   devicePortName = optarg;        break;
            case 'v':   loggingLevel = atoi( optarg );  break;
            case 'c':   synchClocks = FALSE;            break;
            
            default:    showHelp();     break;
        }
    }
}

