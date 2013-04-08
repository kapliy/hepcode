m4_define([VERSION_NUMBER], m4_esyscmd([svn info . | grep Revision | awk '{print $2}' | tr -d '\n']))
dnl m4_define([VERSION_NUMBER], 5)
