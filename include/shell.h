#ifndef __SHELL__
#define __SHELL__
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <sstream>
#include <cstdlib>
#include <thread>
#include <readline/readline.h>
#include <readline/history.h>
// #include <tcl8.5/tcl.h>
#include <tcl.h>

int start_shell (int argc, char * argv[]);
int Tcl_ls (ClientData clientData, Tcl_Interp *interpreter, int argc, char *argv[]);
int Tcl_clear (ClientData clientData, Tcl_Interp *interpreter, int argc, char *argv[]);
int Tcl_vim (ClientData clientData, Tcl_Interp *interpreter, int argc, char *argv[]);
int Tcl_less (ClientData clientData, Tcl_Interp *interpreter, int argc, char *argv[]);
int Tcl_pwd (ClientData clientData, Tcl_Interp *interpreter, int argc, char *argv[]);
int Tcl_subl (ClientData clientData, Tcl_Interp *interpreter, int argc, char *argv[]);
int Tcl_start_gui (ClientData clientData, Tcl_Interp *interpreter, int argc, char *argv[]);
char** command_completion (const char* stem_text, int start, int end);
char*  instersection_name_generator (const char* stem_text, int state);

std::vector <const char*> instersection_names = {
//Commands
	"after", "append", "apply", "array", "auto_execok",
	"auto_import", "auto_load", "auto_load_index", "auto_qualify",
	"binary", "break",
	"case", "catch", "cd", "chan", "clear", "clock", "close", "concat",
	"continue", "coroutine",
	"dict", "encoding",
	"eof", "error", "eval", "exec", "exit", "expr",
	"fblocked", "fconfigure", "fcopy", "file", "fileevent",
	"flush", "for", "foreach", "format",
	"gets", "glob", "global",
	"history",
	"if", "incr", "info", "interp",
	"join",
	"lappend", "lassign", "less", "lindex", "linsert", "list", "llength", "lmap",
	"load", "lrange", "lrepeat", "lreplace", "lreverse", "ls", "lsearch", "lset", "lsort",
	"namespace",
	"open",
	"package", "pid", "proc", "puts", "pwd",
	"quit",
	"read", "regexp", "regsub", "rename", "return",
	"scan", "seek", "set", "socket", "source", "split", "string", "subl", "subst", "switch", "start_gui",
	"tailcall", "tclLog", "tell", "throw", "time", "trace", "try",
	"unknown", "unload", "unset", "update", "uplevel", "upvar",
	"variable", "vim", "vwait",
	"while",
	"yield", "yieldto",
	"zlib"
};
#define print_msg() { 																													\
	std::cout << "  _____  _               _           _    _____          _____    _______          _  "				<< std::endl;	\
	std::cout << " |  __ \\| |             (_)         | |  / ____|   /\\   |  __ \\  |__   __|        | |     "		<< std::endl;	\
	std::cout << " | |__) | |__  _   _ ___ _  ___ __ _| | | |       /  \\  | |  | |    | | ___   ___ | |___  "			<< std::endl;	\
	std::cout << " |  ___/| '_ \\| | | / __| |/ __/ _` | | | |      / /\\ \\ | |  | |    | |/ _ \\ / _ \\| / __| "		<< std::endl;	\
	std::cout << " | |    | | | | |_| \\__ \\ | (_| (_| | | | |____ / ____ \\| |__| |    | | (_) | (_) | \\__ \\ "		<< std::endl;	\
	std::cout << " |_|    |_| |_|\\__, |___/_|\\___\\__,_|_|  \\_____/_/    \\_\\_____/     |_|\\___/ \\___/|_|___/ "	<< std::endl;	\
	std::cout << "                __/ |                                                                     "			<< std::endl;	\
	std::cout << "               |___/                                                                      "			<< std::endl;	\
}


#endif
