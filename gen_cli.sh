cat cli.def | gawk -F '\t' -f gen_cli_ext.awk > clidata.c
