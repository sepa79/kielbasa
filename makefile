
all:
	# cd translation/ && python3 create_translations.py
	"$(HOME)"/apps/c64/oscar64/bin/oscar64 -n -dNOFLOAT -dNOLONG -tf=crt -pp loader.c

lang:
	cd scripts/ && python3 create_translations_yaml_jinja2.py

clean: 
	$(RM) -f loader.asm loader.crt loader.int loader.lbl loader.map
	cd translation/ && $(RM) -f common.h texts*

