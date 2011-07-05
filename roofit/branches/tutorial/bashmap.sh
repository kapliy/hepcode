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

# 2D version
gput() {
    eval a"$1""$2"='$3'
    eval b"$1""$2"='$4'
}
ggeta() {
    eval echo '${'"a$1$2"'#hash}'
}
ggetb() {
    eval echo '${'"b$1$2"'#hash}'
}
gkeys() {
    tnn="$1"
    compgen -A variable "a${tnn}" | sed -e "s#a${tnn}##" | sort
}
