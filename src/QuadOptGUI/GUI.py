from tkinter import *
from tkinter.filedialog import *
from tkinter.messagebox import *
from CustomText import *

filename = None
clipboard = None


def new_file():
    global filename
    text.delete(0.0, END)
    text.insert(0.0, "parameters\nend\n\nvariables\nend\n\n"
                     "minimize\n  quadOpt()\nsubject to\n  A*x == b\n  0 <= x <= 1\nend\n")
    highlight()


def save_file():
    global filename
    try:
        t = text.get(0.0, END)
        f = open(filename, 'w')
        f.write(t.rstrip())
        f.close()
    except:
        save_as()


def save_as():
    f = asksaveasfile(mode='w', defaultextension='.txt')
    t = text.get(0.0, END)
    try:
        f.write(t.rstrip())
    except:
        showerror(title="Something went wrong", message="Unable to save file...")


def open_file():
    f = askopenfile(mode='r')
    t = f.read()
    text.delete(0.0, END)
    text.insert(0.0, t)
    root.title(filename)
    highlight()


def popup(event):
    rightClick.post(event.x_root, event.y_root)


def copy():
    global clipboard
    sel = text.selection_get()
    clipboard = sel


def cut():
    global clipboard
    sel = text.selection_get()
    clipboard = sel
    text.delete(SEL_FIRST, SEL_LAST)


def paste():
    global clipboard
    text.insert(INSERT, clipboard)


def select_all():
    text.tag_add(SEL, "1.0", END)
    text.mark_set(0.0, END)
    text.see(INSERT)


def highlight():
    # make a tag for change the color.
    text.tag_configure("blue", foreground="#48d")

    # apply the tag.
    text.highlight_pattern("parameters", "blue")
    text.highlight_pattern("variables", "blue")
    text.highlight_pattern("minimize", "blue")
    text.highlight_pattern("subject to", "blue")
    text.highlight_pattern("end", "blue")

root = Tk()

# main label
mainLabel = Label(root, text="QuadOpt ALPHA", font="Verdana 12 bold", fg="#305080", bg="#f6f6f6")
mainLabel.pack(expand=True, fill="x")

# sidebar
sideBar = Frame(root, width=150, bg="#f6f6f6", height=400, relief="sunken", borderwidth=2)
sideBar.pack(expand=True, fill="both", side="left", anchor="nw")

# main content area
mainArea = Frame(root, bg="white", width=600, height=400, relief="sunken", borderwidth=2)
mainArea.pack(expand=True, fill="both", side="right")

text = CustomText(mainArea)
text.pack(expand=True, fill="both")

# buttons
problemLabel = Label(sideBar, text="Problem", font="Verdana 12 bold", fg="#305080", bg="#f6f6f6")
problemLabel.pack()
editButton = Button(sideBar, text="Edit", width=15)
editButton.pack()
viewButton = Button(sideBar, text="View", width=15)
viewButton.pack()

blankLabel = Label(sideBar, text=" ", font="Verdana 12 bold", fg="#305080", bg="#f6f6f6")
blankLabel.pack()

codegenLabel = Label(sideBar, text="CODEGEN", font="Verdana 12 bold", fg="#305080", bg="#f6f6f6")
codegenLabel.pack()
cButton = Button(sideBar, text="C code", width=15)
cButton.pack()
matlabButton = Button(sideBar, text="Matlab code", width=15)
matlabButton.pack()


# menu
menuBar = Menu(root)
fileMenu = Menu(menuBar)
fileMenu.add_command(label="New", command=new_file)
fileMenu.add_command(label="Open", command=open_file)
fileMenu.add_command(label="Save", command=save_file)
fileMenu.add_command(label="Save As...", command=save_as)
fileMenu.add_separator()
fileMenu.add_command(label="Quit", command=root.quit)
menuBar.add_cascade(label="File", menu=fileMenu)

editMenu = Menu(menuBar)
editMenu.add_command(label="Copy", command=copy)
editMenu.add_command(label="Cut", command=cut)
editMenu.add_command(label="Paste", command=paste)
editMenu.add_command(label="Select All", command=select_all)
menuBar.add_cascade(label="Edit", menu=editMenu)

rightClick = Menu(root, tearoff=0)
rightClick.add_command(label="Copy", command=copy)
rightClick.add_command(label="Cut", command=cut)
rightClick.add_command(label="Paste", command=paste)
rightClick.add_separator()
rightClick.add_command(label="Select All", command=select_all)

text.bind("<Button-3>", popup)
root.config(menu=menuBar)
new_file()
root.mainloop()