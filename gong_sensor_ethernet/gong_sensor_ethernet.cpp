
//
// gong_sensor
//
// send gong sound level to receiver
//
// author: @jefforulez
//

#include "gong_sensor_ethernet.h"


//
// constants
//

// samples per second
const int SAMPLE_RATE_HZ = 100 ;  

// number of samples used to compute the average
const int NUMBER_READINGS = 5 ; 

// threshold for sending trigger
const int SENSOR_THRESHOLD = 1 ;

// minimum time to wait between triggers
const unsigned long THROTTLE_MS = 1500 ;

const int SENSOR_PIN = A0 ;


//
// networking constants
//

// @jefforulez laptop
// char serverName[]    = "192.168.15.105" ;
// const int serverPort = 46640 ;

// @blip desktop
char serverName[]    = "10.60.121.55" ;
const int serverPort = 46640 ;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED } ;

EthernetClient client ;


//
// global variables
//

int readings[ NUMBER_READINGS ] ;

int index = 0 ;
int total = 0 ;
int average = 0 ;

unsigned long last_sent_ms = 0 ;
unsigned long current_ms = 0 ;

int current_throttle_ms = THROTTLE_MS ;


//
// methods
//

void setup()
{
	Serial.begin( 9600 ) ;
	
	// configure ethernet board
	if ( Ethernet.begin( mac ) == 0 ) {
		while ( true ) ;
	}

	// wait for the board
	delay( 1000 ) ;

	// get and ip address
	IPAddress ip = Ethernet.localIP() ;
		
	// clear the readings array
	for ( int thisReading = 0 ; thisReading < NUMBER_READINGS ; thisReading++ ) {
		readings[ thisReading ] = 0 ;
	}

}

void loop()
{
	// read the current value of the sensor
	int val = analogRead( SENSOR_PIN ) ;
	
	// update the total
	total = total - readings[ index ] ;
	readings[ index ] = val ;
	total = total + readings[ index ] ;

	// compute the average reading
	average = total / NUMBER_READINGS ;
	
	// print the average, with a throttle
	if ( average >= SENSOR_THRESHOLD )
	{
		current_ms = millis() ;
		
		if ( ( current_ms - last_sent_ms ) > current_throttle_ms )
		{
			//
			// TODO:  fix this
			//
			// adjust the next throttle based on the strength of the last strike
			//
			current_throttle_ms = ( average > NUMBER_READINGS )
				? int( average / NUMBER_READINGS ) * THROTTLE_MS
				: THROTTLE_MS 
				;
			if ( current_throttle_ms > THROTTLE_MS * 3 ) {
				current_throttle_ms = THROTTLE_MS * 2 ;
			}
			
			// adjust the last sent marker
			last_sent_ms = current_ms ;	
			
			// connect to the server and send level
			if ( client.connect( serverName, serverPort ) > 0 )
			{
				client.print( average ) ;
				client.stop() ;
			}

		}

	}
	
	// update the readings index
	index = index + 1 ;
	if ( index >= NUMBER_READINGS ) {
		index = 0 ;
	}

	// respect the sample rate
	delay( 1000 / SAMPLE_RATE_HZ ) ;
}


