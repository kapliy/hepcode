function dq2() {
    for user in kapliy; do
	dq2-ls -L ROAMING user.$user.UCNTUP.*.v1_29i/ | grep -v MadGraph
    done
}

dq2 | sort > LIST
