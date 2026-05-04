# -*- coding: utf-8 -*-
from Tkinter import *
import Tkinter, Tkconstants, tkFileDialog
from PIL import ImageTk, Image
import trainSVM
from shutil import copyfile
 
root = Tk()

root.geometry("550x300+300+150")
root.resizable(width=True, height=True)
file_name = ''

def setText(tex):
    var = StringVar()
    label = Label( root, textvariable=var, relief=RAISED )
    var.set(tex)
    label.pack()

def openfn():
    filename = tkFileDialog.askopenfilename(title='open')
    file_name = filename
    return filename

def open_img():
    x = openfn()
    ketqua =  trainSVM.get_class_object(x)
    if ketqua==1:
        ketquacuoi = str('hoa vạn thọ');
    elif ketqua ==2:
        ketquacuoi = str('hoa đại');
    else :
        ketquacuoi = str('hoa cúc huân chương');
    setText("hoa cua bạn là : "+str(ketquacuoi))
    img = Image.open(x)
    img = img.resize((100, 100), Image.ANTIALIAS)
    img = ImageTk.PhotoImage(img)
    panel = Label(root, image=img)
    panel.image = img
    panel.pack()

btn = Tkinter.Button(root, text='open image', command=open_img).pack()



root.mainloop()
