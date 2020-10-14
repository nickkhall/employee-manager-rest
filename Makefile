#### Configuration ################################################################################

_PROJ     = employee-manager-rest#                  # Name of the project and generated executable
_SDIR     = src#                                    # Path to dir of source files (relative to ./)
_HDIR     = include#                                # Path to dir of header files (relative to ./)
_BDIR     = bin#                                    # Path to dir of binary files (relative to ./)
_SSUF     = c#                                      # Suffix of source files
_HSUF     = h#                                      # Suffix of header files
_CC       = gcc#                                    # Compiler to be used
_CFLAGS   = -std=c11 -Wall#                         # Compiler build flags
_LIBS     = -lserc -lsockc -lexplain#
_LIBS_DIR = -L/usr/local/lib#
_SCRIPT   = :#                                      # Any shell script to run before build (replace ':')
_SHELL    = /bin/bash#                              # Shell to be used by makefile
_CARGS    = -I$(_HDIR) -I/usr/include \
						$(_LIBS_DIR) $(_LIBS) $(_CFLAGS)#       # Full set of compiler arguments

_LARGS    = $(_CFLAGS) -I$(_HDIR) -I/usr/include#   #
PURPLE    = \033[0;35m#                             # Encoding of purple color for terminal output
CYAN      = \033[0;36m#                             # Encoding of cyan color for terminal output
NC        = \033[0m#                                # Encoding of no color for terminal output

_DEBUG_DIR   = debug#                               # Debug directory
_DEBUG_EXE   = $(_DEBUG_DIR)/debug#                 # Debug executable name
_DEBUG_FLAGS = -ggdb -O0 -DDEBUG#                   # Debug flags
_DEBUG_PORT  = 12346#                               # Debug port
_DEBUG_CONF  = $(_DEBUG_DIR)/debug_conf.gdb

##### File Lists ##################################################################################

# Header Files
HDRS = $(SOURCES:.c=.d)

# Object Files
OBJS = ${SRC:.c=.o}

# Sources
SRCS = $(wildcard src/*.c)

# Options
OPTS =

##### Dependency Rules ############################################################################
.PHONY: debug clean gdb_debug gdb_debugger
.SILENT: clean

$(_PROJ): 
	$(_CC) main.c $(SRCS) -o $(_BDIR)/$@ $(_CARGS)

$(_BDIR)/%o: $(_SDIR)/%.c
	$(_CC) -c $(SRCS) $(_CARGS) -o $@

$(_SDIR)/%.c: $(_HDIR)/%.h
	$(_CC) -o $< -MM $(_CARGS)

# Delete all binaries and any editor backups of source and header files
clean:
	echo "Employee Manager REST: cleaning up..."
	rm -rf $(_BDIR) $(_SDIR)/*~ $(_HDIR)/*~

# Debug executubale
debug: 
	$(_CC) $(_DEBUG_FLAGS) main.c $(SRCS) -o $(_DEBUG_EXE) $(_CARGS)

# GDB debug exectubale
gdb_debug:	
	gdb -x $(_DEBUG_CONF)

# GDB debugger server
gdb_debugger:	
	$(shell tmux splitw -v "gdbserver localhost:$(_DEBUG_PORT) $(_DEBUG_EXE)") 
	echo "Employee Manager REST: Debugging on port $(_DEBUG_PORT)..."
