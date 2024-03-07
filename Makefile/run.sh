#!/bin/bash

program_name="/root/MT\ Series/test"
max_attempts=99
attempt=1

run_rvd() {
	echo "Running ./test"
	/root/MT\ Series/test
	return $?
}

if pgrep -f "$program_name" > /dev/null; then
	echo "Program is running"
else
	echo "Program is not running"

	while [ $attempt -le $max_attempts ]; do
		echo "Attempt $attempt: Running ./test"

		run_rvd
		exit_status=$?

		if [ $exit_status -eq -1 ]; then
			echo "./test returned -1, Restarting"
		else
			echo "./test exit status: $exit_status"
		fi

		((attempt++))
	done

	if [ $attempt -gt $max_attempts ]; then
		echo "Restart Failed"
	else
		echo "Restart Successfully"
	fi
fi
