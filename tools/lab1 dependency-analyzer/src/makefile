ifndef PIN_ROOT
$(error PIN_ROOT is not defined. Please set it to your Pin installation directory.)
endif

CONFIG_ROOT := $(PIN_ROOT)/source/tools/Config
include $(CONFIG_ROOT)/makefile.config
include $(CONFIG_ROOT)/makefile.default.rules

TOOL_NAME := dependency_analyzer

all: $(objdir)$(TOOL_NAME)$(PINTOOL_SUFFIX)