#!/usr/bin/env perl

use strict ;

package GongBot ;

use base qw( Bot::BasicBot ) ;

use Data::Dumper ;

use IO::File ;
use IO::Select ;
use IO::Socket::INET ;

sub init
{
	my ( $self ) = @_ ;

	print STDERR "GongBot::init()\n" ;
	print Dumper( $self ), "\n" ;
	
	return 1 ;
}

sub connected 
{
	my ( $self ) = @_ ;

	print STDERR "GongBot::connected()\n" ;
	
	my $io_select = IO::Select->new() ;

	my $data_source = $self->{ '_gongbot' }{ 'data_source' } ;

	if ( -e $data_source && -c _ )
	{
		print STDERR "[info] data source is a character special file, data_source : $data_source\n" ;

		my $fh = IO::File->new( $data_source ) ;

		if ( ! $fh ) {
			print STDERR "[error] unable to open data source, data_source : $data_source\n" ;
			$self->shutdown( '' ) ;
		}
	
		$io_select->add( $fh ) ;
	}
	elsif ( $data_source =~ m/^(.*?):(\d+)$/ )
	{
		print STDERR "[info] attempting to open socket, data_source : $data_source\n" ;
	
		my $sock = IO::Socket::INET->new(
			LocalAddr => $1,
			LocalPort => $2,
			Listen    => 5,
			Proto     => 'tcp'
		) ;

		if ( ! $sock ) {
			print STDERR "[error] unable to open socket, data_source : $data_source\n" ;
			$self->shutdown( '' ) ;
		}
	
		$io_select->add( $sock ) ;
	}
	else
	{
		print STDERR "[error] invalid data source, data_source : $data_source\n" ;
		$self->shutdown( '' ) ;
	}

	$self->{ '_gongbot' }{ 'io_select' } = $io_select ;

	return ;
}

=pod
sub said 
{
	my ( $self, $msg ) = @_ ;
	print STDERR Dumper( $msg ), "\n" ;
	return ;
}
=cut

sub tick
{
	my ( $self ) = @_ ;

	my $io_select = $self->{ '_gongbot' }{ 'io_select' } ;
	
	while ( my @ready = $io_select->can_read(0) ) 
	{
		foreach my $fh ( @ready ) 
		{
			if ( ref $fh eq 'IO::File' )
			{
				my $line = <$fh> ;
				chomp $line ;

				if ( $line =~ m/^\s*(\d+)\s*$/ )
				{
					$self->_report_gong_strike( $1 ) ;
				}
				elsif ( $line ) 
				{
					print STDERR "unexpected data, line : '$line'\n" ;
				} 
			}
			elsif ( ref $fh eq 'IO::Socket::INET' )
			{				
				if ( my $client = $fh->accept() ) 
				{
					$client->autoflush(1) ;
					
					if ( my $line = <$client> ) 
					{
						if ( $line =~ m/^\s*(\d+)\s*$/ ) 
						{
							$self->_report_gong_strike( $1 ) ;
						}
					}
					close $client ;
				}
			}
			else
			{
				print STDERR "[error] unexpected filehandle type\n" ;
			}
			
		}
	}

	return 1 ;
}

sub _report_gong_strike
{
	my ( $self, $val ) = @_ ;

	print STDERR "GongBot::_report_gong_strike( $val )\n" ;

	my $body = ( $val >= 50 ) 
			 ? "*** GONG ***" 
			 : "... ding ..."
			 ;

	$self->say(
		channel => ( $self->channels )[0],
		body    => $body,
	) ;

}


1; 

__END__

