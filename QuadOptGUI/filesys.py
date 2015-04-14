from GUI import *

def new_file(event=None):
    global filename
    global edit_ata

    text.delete(0.0, END)
    text.insert(0.0, start_file)
    edit_data = text.get(0.0, END)
    root.title("New file")
    highlight()


def save_file(event=None):
    global filename
    try:
        t = text.get(0.0, END)
        f = open(filename, 'w')
        f.write(t.rstrip())
        f.close()
    except:
        save_as()


def save_as(event=None):
    global filename
    try:
        f = asksaveasfile(mode='w', defaultextension='.qopt')
        t = text.get(0.0, END)
        filename = f.name
        f.write(t.rstrip())
        root.title(filename)
    except:
        showerror(title="Something went wrong",
                        message="Unable to save file...")


def open_file(event=None):
    global filename
    try:
        f = askopenfile(mode='r')
        t = f.read()
        filename = f.name

        text.delete(0.0, END)
        text.insert(0.0, t)
        root.title(filename)
        highlight()
    except:
        pass


def popup(event):
    rightClick.post(event.x_root, event.y_root)


def copy(event=None):
    global clipboard
    sel = text.selection_get()
    clipboard = sel


def cut(event=None):
    global clipboard
    sel = text.selection_get()
    clipboard = sel
    text.delete(SEL_FIRST, SEL_LAST)


def paste(event=None):
    global clipboard
    try:
        text.insert(INSERT, clipboard)
    except:
        pass
    highlight()


def select_all(event=None):
    text.tag_add(SEL, "1.0", END)
    text.mark_set(0.0, END)
    text.see(INSERT)
    return 'break'


def deselect_all(event=None):
    text.tag_remove(SEL, "1.0", END)