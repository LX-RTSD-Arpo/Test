#!/bin/bash

read -p "Enter Radar IP Address: " arg1

program_name="./rvd-v1.0.0b0 $arg1"
max_attempts=99
attempt=1

run_rvd() {
	echo "Starting rvd-v1.0.0b0..."
	/root/MT\ Series/test
	return $?
}

if pgrep -f "$program_name" > /dev/null; then
	echo "Program is running"
else
	echo "Program is not running"

	while [ $attempt -le $max_attempts ]; do
		echo "Attempt $attempt"

		run_rvd
		exit_status=$?

		if [ $exit_status -eq -1 ]; then
			echo "rvd-v1.0.0b0 returned -1, Restarting"
		else
			echo "rvd-v1.0.0b0 exit status: $exit_status"
		fi

		((attempt++))
	done

	if [ $attempt -gt $max_attempts ]; then
		echo "Restart Failed"
	else
		echo "Restart Successfully"
	fi
fi
