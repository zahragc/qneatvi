#ifndef API_H
#define API_H

#ifdef __cplusplus
extern "C" {
#endif

void api_MainWindow(void);
int api_cols(void);
int api_rows(void);
void  api_write(char * s);
void  api_debug(char * s);
void  api_debugi(int n);
void api_pos(int r , int c);
void api_kill(void);
void api_room(int n);
void api_init(void);
int api_read(char *ibuf, int readsize);
void api_minimize(void);

void api_write_enter(void);
void api_write_tilda(char * s);
void api_write_msg(char * s);
void api_write_output(void);

void api_close_mainwindow(void);
void api_clear_all(void);

void api_mode_normal(void);
void api_mode_insert(void);
void api_mode_ex(void);
void api_mode_after_cmd(void);

void api_alarm_notsave(char *c);
#ifdef __cplusplus
}
#endif

#endif // API_H
