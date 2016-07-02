#!bin/sh
# /etc/init.d/rcontd.sh

### BEGIN INIT INFO
# Provides:			rcont startup
# Required-Start:	$all
# Required-Stop:	$all
# Default-Start:	2 3 4 5
# Default-Stop:		0 1 6
# Short-Description: start rcont
# Description:		start rcont daemon (relay manager)
### END INIT INFO

#rcont stop function
rcont_stop(){
	pkill rcont
}

#rcont start function
rcont_start(){
	rcont_stop
	rcont
}

case $1 in
	start)
		rcont_start
		;;
	
	stop)
		rcont_stop
		;;
	
	restart)
		rcont_start
		;;
		
	*)
		usage
esac

exit 0
