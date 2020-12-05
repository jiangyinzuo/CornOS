# list all files in some directories: (#directories, #types)
list_files = $(filter $(if $(2),$(addprefix %.,$(2)),%),\
		     $(wildcard $(addsuffix $(SLASH)*,$(1)))

# get .o obj files: (#files[, packet])
to_obj = $(addprefix $(OBJDIR)$(SLASH)$(if $(2),$(2)$(SLASH)),\
		 $(addsuffix .o,$(basename $(1))))
