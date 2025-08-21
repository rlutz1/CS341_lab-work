# C Lab Work for CS 341

Because my dumbass lost all my 241 work so we're starting over!

## Some notes on compilation

Writing this here so maybe I don't have to figure it out again.

Two main methods for working:

1. Work directly through SSH on the CS machines and compile there.
2. Get GCC up and running on Windows.

### SSH on CS Machines

Be careful with this because you will likely lose your stupid work if you don't back it up somewhere (see above self deprication).

1. SSH into the CS machines using PuTTY (easiest):

+ "Host Name" is either <a href = "https://www.cs.unm.edu/computer-facilities/remote-login.html">moons.cs.unm.edu or trucks.cs.unm.edu</a>. I usually use moons because I'm a girl and girls don't like trucks.
+ login with UNM netID and password, and should be all good.

2. Normal Linux Ubuntu environment; vi to edit files; all typical commands; gcc prebuilt in, no work needed.

3. Run executables prefixed with "./" (ie, "./a.out", as is the default binary compilation file).

Again, this is fine, but BE SURE TO BACKUP ELSEWHERE AT SOME POINT!

### GCC on Windows

Easiest thing: MinGW-w64.

1. Install minGW-w64 (<a href = "https://phoenixnap.com/kb/install-gcc-windows">found this tutorial to work just fine and binaries just *worked*</a>. Path var tutorial as a bonus while you're there). Lovely tool suite for Windows.

2. Should now be able to gcc to your hearts content C files.

3. One thing to note: executables are made as .exe executable files. Default is "a.exe". renaming with -o yields a "[name].exe". Runnable by just "[name].exe" on command line.

This will give you a lot more freedom if you're a nasty Windows user like me but needs to work locally on C code easily. No known limitations to note right now.

## Some notes on git and GitHub

I set up my lab work just now following <a href = "https://docs.github.com/en/migrations/importing-source-code/using-the-command-line-to-import-source-code/adding-locally-hosted-code-to-github">this tutorial</a> and it worked just fine.

They mainly push the git bash terminal, but everything remarkably ran okay in command prompt.

But, highly recommend, is an easy way to save work.

Recommended workflow:

+ Work locally.
+ Push changes to GitHub as a backup repository on a remote machine.
+ SFTP the files to the CS machines as needed.

## Some notes on SFTP

There's a video posted on how to do this on Canvas, but of course it's not shareable, so here:

1. Command line: `sftp [UNM netId]@b146-[any machine number, ie, 32].cs.unm.edu`
2. Accept any risky nonsense
3. Enter password for CS account

Should be in. Now you can use `get` and `put` commands to push files from your local machine to the CS account. ie, from the directory your file is in, SFTP in, and you should be able to run:

`put [filename here]`

And it should copy the file to your CS account, viewable from the machines. In turn, `get` will pull files from your CS account to your local machine. Happy hunting!

 
