function write_string(str) {
	printf ", %d", length(str);
	for(j = 1; j<=length(str); j++)
		printf ", '%s'", substr(str, j, 1);
}
BEGIN { cntr = 1; tord = 0;  printf "#include \"clidata.h\"\n#include \"cli_defs.h\"\nunsigned char cli_tab[] = {\n"; printf "" > "clidata.h"; }
{
	line++;
	if(length($0) == 0)  next;
	if(substr($0, 1, 1) == "#") next;
	for(level = 1; level<=NF; level++)
		if(length($level) > 0)
			break;
	
	word = $level;
	unid = $(level + 1);
	help = $(level + 2);
	
	split(unid, a, "=");
	unid = a[1];
	intval = a[2];
	
	level--;
	
	ptr = "";
	for(i = 0; i<level; i++)
		ptr = ptr "." tree[i];
	
#	printf "read %s level %s, currrent is %s\n", word, level, ptr;
	
	if(substr(word, 1, 1) == "#")
	{
		tables[ptr ":jump"] = substr(word, 2);
	}
	else
	{
		if(!unid)
			printf "#warning syntax error in line %d: %s\n", line, $0 >> "clidata.h";

#		printf "assigning level %s word %s parent %s\n", level, word, ptr;
		tree[level] = word;

		nx = ptr "." word;
		tables[ptr ":terminal"] = 0;
		tables[nx ":terminal"] = 1;
		tables[nx ":parent"] = ptr;
		tables[nx ":ent"] = word;
		tables[nx ":id"] = unid;
		tables[nx ":help"] = help;
		if(length(intval))
			tables[nx ":intval"] = intval;
		else
			tables[nx ":intval"] = "";
				
		table_order[tord++] = nx;
		if(!(unid in symbols))
		{
			symbols[unid] = 1;
			printf "#define %s %d\n", unid, cntr++ > "clidata.h";
		}
	}
}
function get_len(tab) {

	len  = 6 + 2 + length(tables[tab ":help"]);
	if(substr(tables[tab ":ent"], 1, 1) != "<")
		len  += 1 + length(tables[tab ":ent"]);
	
	if(tables[tab ":intval"] != "")
		len += 2; 
		
	tables[tab ":len"] = len;
	return len;

}
function find_offsets(offs, parent, i, tab) {
	tables[parent ":offs"] = offs;
#	printf "Offset of %s is %d\n", parent, offs;
	for(i = 0; i<tord; i++)
	{
		tab = table_order[i];
		if(tables[tab ":parent"] == parent)
		{
			len = get_len(tab);
			offs += len;
		}
	}
	if(tables[parent ":terminal"])
		return offs;
	offs += 1;

	for(i = 0; i<tord; i++)
	{
		tab = table_order[i];
		if(tables[tab ":parent"] == parent)
			offs = find_offsets(offs, tab);
	}
	return offs;
}
function list_table(parent, i, tab) {
	printf "/* Listing of table: %s */\n", parent; 
	for(i = 0; i<tord; i++)
	{
		tab = table_order[i];
		if(tables[tab ":parent"] == parent)
		{
			
			help = tables[tab ":help"];
			len = tables[tab ":len"];
			id = tables[tab ":id"];
			ent = tables[tab ":ent"];

			jmp = tables[tab ":jump"];
			not = tab;
			
			if(length(jmp) > 0 && !tables[jmp ":terminal"])
				not = jmp;
			else if(tables[tab ":terminal"])
			{
				not = "End";
				nol = 255;
				noh = 255;
				nohh = 255;
			}

		
			if(not != "End")
			{
				no = tables[not ":offs"];
				nohh = no / 65536;
				noh = (no / 256) % 256;
				nol = no % 256;
			}
				
			if(ent == "<num>") 
				type = "TOK_NUM";
			else if(ent == "<string>")
				type = "TOK_STRING";
			else if(ent == "<word>")
				type = "TOK_WORD";
			else if(ent == "<ip>")
				type = "TOK_IP";
			else if(ent == "<CR>")
				type = "TOK_CR";
			else if(length(tables[tab ":intval"]))
				type = "TOK_KEYWORD_INTVAL";
			else
				type = "TOK_KEYWORD";
		
			printf "	%d, %s >> 8, %s & 0xff, %s, %d, %d, %d /* %s */ ", len, id, id, type, nohh, noh, nol, not;
			write_string(help);
			if(type == "TOK_KEYWORD" || type == "TOK_KEYWORD_INTVAL")
				write_string(ent);
			if(type == "TOK_KEYWORD_INTVAL")
			{
				iv = tables[tab ":intval"];
				ivh = iv / 256;
				ivl = iv % 256;
				printf ", %d, %d", ivh, ivl;
			}
			
			printf ",\n";
		}
	}
	printf "TOK_END, \n\n";

	for(i = 0; i<tord; i++)
	{
		tab = table_order[i];
		if(tables[tab ":parent"] == parent && tables[tab ":terminal"] == 0)
			list_table(tab);
	}
	return;
}

END {
	find_offsets("");
	list_table("");
	printf  "};\n";
}