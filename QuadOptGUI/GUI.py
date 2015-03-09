from tkinter import *
from tkinter.filedialog import *
from tkinter.messagebox import *
from CustomText import *
from parser import *

filename = None
clipboard = None


def new_file(event=None):
    global filename
    text.delete(0.0, END)
    text.insert(0.0, "parameters\n\nend\n\nvariables\n\nend\n\n"
                     "minimize\n  quadOpt()\nsubject to\n  A*x == b\n  0 <= x <= 1\nend\n")
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
    f = asksaveasfile(mode='w', defaultextension='.qopt')
    t = text.get(0.0, END)
    try:
        f.write(t.rstrip())
    except:
        showerror(title="Something went wrong", message="Unable to save file...")


def open_file(event=None):
    f = askopenfile(mode='r')
    t = f.read()

    text.delete(0.0, END)
    text.insert(0.0, t)
    root.title(filename)
    highlight()


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


def highlight(event=None):
    # make a tag for change the color.
    text.tag_configure("blue", foreground="#48d")

    # apply the tag.
    text.highlight_pattern("parameters", "blue")
    text.highlight_pattern("variables", "blue")
    text.highlight_pattern("minimize", "blue")
    text.highlight_pattern("subject to", "blue")
    text.highlight_pattern("end", "blue")

def generate_c(event=None):
    print("Generate C code")
    read_file()

def generate_mat(event=None):
    print("Generate Matlab code")

def view_problem(event=None):
    print("View problem")

root = Tk()

# main label
mainLabel = Label(root, text="QuadOpt Alpha", font="Verdana 12 bold", fg="#305080", bg="#f6f6f6")
mainLabel.pack(expand=False, fill="x")

# sidebar
sideBar = Frame(root, width=150, bg="#f6f6f6", height=400, relief="sunken", borderwidth=2)
sideBar.pack(expand=False, fill="both", side="left", anchor="nw")

# main content area and scrollbar
scrollbar = Scrollbar()
scrollbar.pack(side=RIGHT, fill=Y)

mainArea = Frame(root, bg="white", width=600, height=400, relief="sunken", borderwidth=2)
mainArea.pack(expand=True, fill="both", side="right")

text = CustomText(mainArea, yscrollcommand=scrollbar.set, undo=True)
text.pack(expand=True, fill="both")

scrollbar.config(command=text.yview)

# buttons
problemLabel = Label(sideBar, text="Problem", font="Verdana 12 bold", fg="#305080", bg="#f6f6f6")
problemLabel.pack()
editButton = Button(sideBar, text="Edit", width=15)
editButton.pack()
viewButton = Button(sideBar, text="View", width=15, command=view_problem)
viewButton.pack()

blankLabel = Label(sideBar, text=" ", font="Verdana 12 bold", fg="#305080", bg="#f6f6f6")
blankLabel.pack()

codegenLabel = Label(sideBar, text="CODEGEN", font="Verdana 12 bold", fg="#305080", bg="#f6f6f6")
codegenLabel.pack()
cButton = Button(sideBar, text="C code", width=15, command=generate_c)
cButton.pack()
matlabButton = Button(sideBar, text="Matlab code", width=15, command=generate_mat)
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

# Keybindings
text.bind("<Button-3>", popup)
text.bind("<Button-1>", deselect_all)
text.bind("<Control-c>", copy)
text.bind("<Control-v>", paste)
text.bind("<Control-x>", cut)
text.bind("<Control-s>", save_file)
text.bind("<Control-S>", save_as)
text.bind("<Control-o>", open_file)	
text.bind("<Control-n>", new_file)
text.bind("<Control-a>", select_all)
root.bind("<KeyPress>", highlight)

root.config(menu=menuBar)
root.wm_title("QuadOpt Solver")
new_file()
root.mainloop()