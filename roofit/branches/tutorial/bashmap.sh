hput() {
    eval "$1""$2"='$3'
}

hget() {
    eval echo '${'"$1$2"'#hash}'
}
hkeys() {
    tnn="$1"
    compgen -A variable "${tnn}" | sed -e "s#${tnn}##"
}
