#!/usr/bin/env perl

use strict ; 

use GongBot ;

MAIN:
{
	usage() if ( ! $ARGV[3] ) ;
	
	my $channel = $ARGV[4] || '#gongbot' ;
	
	print STDERR "starting GongBot\n" ;
	print STDERR "  server: $ARGV[0]\n" ;
	print STDERR "  port: $ARGV[1]\n" ;
	print STDERR "  ssl: $ARGV[2]\n" ;
	print STDERR "  data_filename: $ARGV[3]\n" ;
	print STDERR "  channel: $channel\n" ;

	my $gongbot = GongBot->new( 
		server   => $ARGV[0],
		port     => $ARGV[1],
		ssl      => $ARGV[2],
		channels => [ $channel ],
		nick     => 'gongbot', 
		#
		_gongbot  => { 
			data_source => $ARGV[3], 
		},
	) ;

	$gongbot->run() ;
	
}

sub usage
{
	print "gong_bot.pl <server> <port> <ssl> <serial_dev>|<listen_ip:port> <channel>\n" ;
	exit 1 ;
}

exit ;

__DATA__

