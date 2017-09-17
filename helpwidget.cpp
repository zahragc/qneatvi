#include "helpwidget.h"

HelpWidget::HelpWidget(QWidget * parent) : QTabWidget(parent){
    setTabText();
    setWindowTitle("Help");
    addTab(&confArea, "Configuration");
    addTab(&comArea, "Commands");
    addTab(&optAria, "Options");
    addTab(&markArea, "Marks and buffers");
    resize(600,300);
    move(0,0);
}

void HelpWidget::setTabText(void){
    configuration.setText(
                "<p>Edit <b>conf.h</b> to adjust syntax highlighting rules and text direction patterns.</p>"
                "<p>To define a new keymap, create a new array in <b>kmap.h</b>, like "
                "kmap_fa, and add it to kmaps array in the same header (the first entry "
                "of the new array specifies its name).</p>"
                "<p>The current keymap may be changed with <b style=color:red>:cm</b> ex command.</p>"
                "<p>When in input mode, <b style=color:blue>^e</b> activates the English kaymap and <b style=color:blue>^f</b> switches to the "
                "alternate keymap (the last keymap specified with <b style=color:red>:cm</b>).</p>"
                );
    configuration.setWordWrap(true);
    configuration.setAlignment(Qt::AlignLeft | Qt::AlignTop);
    configuration.setMargin(20);
    confArea.setWidget(&configuration);
    confArea.setBackgroundRole(QPalette::Light);
    confArea.setWidgetResizable(true);
    //--------------------------
    commands.setText(
                "<b>Commands not available in ex(1):</b><br><br>"

                "<b style=color:red>:cm</b>[<font color=green>ap</font>][<font color=green>!</font>] [<font color=green>kmap</font>]<br>"
                    "Without <font color=green>kmap</font>, prints the current keymap name.<br>"
                    "When <font color=green>kmap</font> is specified, sets the alternate keymap to "
                    "<font color=green>kmap</font> and, unless <b style=color:green>!</b> is given, switches to this keymap.<br><br>"

                "<b style=color:red>:ft</b> [<font color=green>filetype</font>]<br>"
                   "Without <font color=green>filetype</font>, prints the current file type.<br>"
                   "When <font color=green>filetype</font> is specified, sets the file type of the "
                  "current ex buffer.<br><br>"

               "<b>New key mappings:</b><br>"
               "<b style=color:red>^a</b> in normal mode: searches for the word under the cursor.<br>"
               "<b style=color:blue>^p</b> in insert mode: inserts the contents of the default yank buffer.<br>"
               "<b style=color:red>zL</b>, <b style=color:red>zl</b>, <b style=color:red>zr</b>, and <b style=color:red>zR</b> in normal mode: change the value of td option.<br>"
               "<b style=color:blue>^e</b> and <b style=color:blue>^f</b> in insert mode: switch to the English and alternate keymap.<br>"
               "<b style=color:red>ze</b> and <b style=color:red>zf</b> in normal mode: switch to the English and alternate keymap.<br>"
               "<b style=color:red>gu</b>, <b style=color:red>gU</b>, and <b style=color:red>g~</b> in normal mode: switch character case.<br>"
               "<b style=color:red>^l</b> in normal mode: updates terminal dimensions (after resizing it).<br>"
                );
    commands.setWordWrap(true);
    commands.setAlignment(Qt::AlignLeft | Qt::AlignTop);
    commands.setMargin(20);
    comArea.setWidget(&commands);
    comArea.setBackgroundRole(QPalette::Light);
    comArea.setWidgetResizable(true);
    //-------------------------------
    options.setText(
                "<p>To improve neatvi's performance, shaping, character reordering, and "
                "syntax highlighting can be disabled by defining the <b>EXINIT</b> environment "
                "variable as \"<b>set noshape</b> | <b>set noorder</b> | <b>set nohl</b> | <b>set td=+2</b>\".</p>"

                "<b>Options supported in neatvi:</b><br>"

                "<b style=color:red>td</b>, <b style=color:red>textdirection</b><br>"
                  "Current direction context.  The following values are meaningful:<br>"
                  "* +2: always left-to-right.<br>"
                  "* +1: follow <b>conf.h</b>'s dircontexts[]; left-to-right for others.<br>"
                  "* -1: follow <b>conf.h</b>'s dircontexts[]; right-to-left for others.<br>"
                  "* -2: always right-to-left.<br>"
                "<b style=color:red>shape</b><br>"
                  "If set (default), performs Arabic/Farsi letter shaping.<br>"
                "<b style=color:red>order</b><br>"
                  "If set, reorder characters based on the rules definedin <b>conf.h</b>.<br>"
                "<b style=color:red>hl</b>, <b style=color:red>highlight</b><br>"
                  "If set (default), text will be highlighted based on syntax highlighting rules in <b>conf.h</b>.<br>"
                "<b style=color:red>ai</b>, <b style=color:red>autoindent</b><br>"
                  "As in vi(1).<br>"
                "<b style=color:red>aw</b>, <b style=color:red>autowrite</b><br>"
                  "As in vi(1).<br>"
                "<b style=color:red>ic</b>, <b style=color:red>ignorecase</b><br>"
                 "As in vi(1)."
                );
    options.setWordWrap(true);
    options.setAlignment(Qt::AlignLeft | Qt::AlignTop);
    options.setMargin(20);
    optAria.setWidget(&options);
    optAria.setBackgroundRole(QPalette::Light);
    optAria.setWidgetResizable(true);
    //---------------------------------
    marksAndBuffers.setText(
                "<b>Special marks:</b><br>"
                "<b style=color:red>*</b> the position of the previous change<br>"
                "<b style=color:red>[</b> the first line of the previous change<br>"
                "<b style=color:red>]</b> the last line of the previous change<br><br>"

                "<b>Special yank buffers:</b><br>"
                "<b style=color:red>/</b> the previous search keyword<br>"
                "<b style=color:red>:</b> the previous ex command<br>"
                );
    marksAndBuffers.setWordWrap(true);
    marksAndBuffers.setAlignment(Qt::AlignLeft | Qt::AlignTop);
    marksAndBuffers.setMargin(20);
    markArea.setWidget(&marksAndBuffers);
    markArea.setBackgroundRole(QPalette::Light);
    markArea.setWidgetResizable(true);
}
