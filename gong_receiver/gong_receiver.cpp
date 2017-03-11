
//
// gong_receiver
//
// receive gong sound level from transmitter
//
// author: @jefforulez
//

#include "gong_receiver.h"

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED } ;

const int ledPin = 7 ;

// rulez.com
// char serverName[]    = "107.21.151.112" ; 
// const int serverPort = 8080 ;

// laptop at home
char serverName[]    = "192.168.15.105" ; // laptop at home
const int serverPort = 8080 ;

EthernetClient client ;
int incomingByte ;

void setup() 
{	
	Serial.begin( 9600 ) ;
	
	pinMode( ledPin, OUTPUT ) ;
	
	// startup indicator	
	digitalWrite( ledPin, HIGH ) ;

	// configure ethernet board
	if ( Ethernet.begin( mac ) == 0 ) {
		while ( true ) ;
	}

	// wait for the board
	delay( 1000 ) ;

	// get and ip address
	IPAddress ip = Ethernet.localIP() ;

	// startup indicator
	digitalWrite( ledPin, LOW ) ;		
}

void loop() 
{				
	// check for serial input
	if ( Serial.available() < 1 ) {
		return ;
	}

	// data indicator
	digitalWrite( ledPin, HIGH ) ;

	incomingByte = Serial.read() ;
	
	// connect to the server and send level
	if ( client.connect( serverName, serverPort ) > 0 )
	{
		client.println( incomingByte ) ;
		client.stop() ;
	}

	// data indicator
	digitalWrite( ledPin, LOW ) ;	
		
}
