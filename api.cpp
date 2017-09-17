#include "api.h"
#include "mainwindow.h"

static MainWindow * a;
void api_MainWindow(void){
    a = new MainWindow();
    a->show();
}
void api_init(void){ a->getTextEdit()->init(); }
int api_cols(void){ return a->getTextEdit()->cols(); }
int api_rows(void){ return a->getTextEdit()->rows(); }
int api_read(char  *ibuf, int readsize){
    return a->getTextEdit()->read(ibuf , readsize);
}
void api_write(char * s){ a->getTextEdit()->write(s); }
void api_pos(int r , int c){ a->getTextEdit()->pos(r,c); }
void api_kill(void) { a->getTextEdit()->kill(); }
void api_room(int n){ a->getTextEdit()->room(n); }
void api_write_enter(void){ a->getTextEdit()->writeEnter(); }
void api_write_tilda(char * s){ a->getTextEdit()->writeTilda(s); }
void api_write_msg(char * s){ a->getTextEdit()->writeMsg(s); }
void api_write_output(void){ a->getTextEdit()->writeOutput(); }

void api_minimize(void){ a->minimize(); }

void api_mode_normal(void){ a->getTextEdit()->enterCommandMode(); }
void api_mode_insert(void){ a->getTextEdit()->enterInsertMode(); }
void api_mode_ex(void){ a->getTextEdit()->enterExMode(); }
void api_mode_after_cmd(void){ a->getTextEdit()->enterAfterCmdMode(); }

void  api_debugi(int n){ a->debug(n); }
void  api_debug(char * s){ a->debug(s); }
void api_close_mainwindow(void){ delete  a;}
void api_clear_all(void){ a->getTextEdit()->clear(); }
void api_alarm_notsave(char *c){
    if(a->getTextEdit()->getGuiEvent())
        a->getTextEdit()->alarmNotSave(c);
}
