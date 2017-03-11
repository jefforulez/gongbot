
var irc = require( 'irc' ) ;
var net = require( 'net' ) ;

//
// constants
//

const IRC_SERVER = 'irc.corp.pokkari.net' ;
const IRC_CHANNEL = '#bliptv' ;

const IRC_HANDLE = 'gongbot' ;
const IRC_REAL_NAME = 'gongbot' ;
const IRC_DEBUG = true ;

const SERVER_PORT = 46640 ;


//
// configure the irc client
//

var irc = new irc.Client(
	IRC_SERVER,
	IRC_HANDLE, 
	{
		channels : [ IRC_CHANNEL ],
		realName : IRC_REAL_NAME,
		//
		showErrors : true,
		debug : IRC_DEBUG,
	}
) ;

irc.addListener(
 	'registered', 
	function ( message ) {
    	console.log( "[registered] " ) ;
    	console.log( message ) ;
	}
) ;

irc.addListener(
 	'names', 
	function ( channel, nicks ) {
    	console.log( "[names] channel : " + channel + ", nicks : " + nicks ) ;
	}
) ;


irc.addListener(
	'message', 
	function ( from, to, message ) {
    		console.log( "[message] from : " + from + ', to : ' + to + ', message : ' + message ) ;
	}
) ;


irc.addListener(
	'pm',
	function ( from, message ) 
	{
    		console.log( "[pm] from : " + from + ', message : ' + message ) ;
	}
) ;

irc.addListener(
 	'error', 
	function ( message ) {
    		console.log( "[error] " + message ) ;
	}
) ;



//
// configure the server
//

var server = net.createServer(
	function ( client ) 
	{
		client.on( 
			'data', 
			function ( data ) 
			{ 
				if ( data > 5 ) {
					console.log( "[server] gong, data : '" + data + "'" ) ; 
					irc.say( IRC_CHANNEL, '*** GONG ***  (' + data + ')' ) ;	
				}
				else if ( data > 1 ) {
					console.log( "[server] ding, data : '" + data + "'" ) ; 
					irc.say( IRC_CHANNEL, '... ding ...  (' + data + ')' ) ;
				}
				else {
					// console.log( "[server] -data : '" + data + "'" ) ; 
				}
			} 
		) ;
	}
) ;

server.listen(
	SERVER_PORT, 
	function () {
		console.log( '[server] bound' ) ;
	}
) ;

//
//

console.log( 'gongbot.js started' ) ;
