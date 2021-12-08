all: compile


compile:
	@cd codigo/ ; \
	for var in gag gas gap pag pas pap sag sas sap ; do \
		make BP=$$var ; \
	done

relatorio:
	@cd relatorio/ ; \
		make ; \