#!/bin/bash
set -m

function _term {
	kill -TERM "$child" 2>/dev/null
	wait "$child"
}
trap _term SIGTERM SIGINT

cd ~/.openea
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/.openea
if [[ $2 == "run" ]]; then
	if [[ $OMPI_COMM_WORLD_RANK == "0" ]]; then
		cd "$1"
		~/.openea/exec ~/.openea/"$3" "${@:4}" -c &
	else
		~/.openea/exec "${@:3}" -ld= -c &
	fi
elif [[ $2 == "exec" ]]; then
	~/.openea/exec "${@:3}" &	
fi
child=$! 
wait "$child"
