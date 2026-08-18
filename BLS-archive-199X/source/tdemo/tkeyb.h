void InitKeyboard(void (*kfunc)(void));
void TerminateKeyboard();
extern void (*keyboardhandler)(void);  //pointer to our handler

