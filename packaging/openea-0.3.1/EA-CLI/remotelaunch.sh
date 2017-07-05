#!/bin/bash
set -m

function _term {
	kill -TERM "$child" 2>/dev/null
	wait "$child"
}
trap _term SIGTERM SIGINT

cd ~/.openea/cluster
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/.openea/cluster
if [[ $2 == "run" ]]; then
	if [[ $OMPI_COMM_WORLD_RANK == "0" ]]; then
		cd "$1"
		openea run ~/.openea/cluster/"$3" "${@:4}" -c &
	else
		openea run "${@:3}" -ld= -c &
	fi
elif [[ $2 == "exec" ]]; then
	~/.openea/cluster/exec "${@:3}" &	
fi
child=$! 
wait "$child"
