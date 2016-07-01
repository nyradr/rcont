#!bin/sh

SERVICE_NAME="rcont"

DIR=/home/pi/git/rcont
NAME=rcont
RCONT=$DIR/$NAME

#rcont stop function
rcont_stop(){
	pkill $NAME
}

#rcont start function
rcont_start(){
	rcont_stop
	exec $RCONT
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
