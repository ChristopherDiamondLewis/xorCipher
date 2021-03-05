Hello!

Here are some simple instructions for my encryptor and testing the files included.

If you run the makefile or the compile command you can pipe the files named 
'appleEncrypted.jpg' and 'diceEncrypted.jpg' while using the keyfile provided and redirecting 
output to  any .jpg (or whatever the correct extension of the original file is). 
You will see the end result of 'appleEncrypted.jpg' and 'diceEncrypted.jpg' 
being the same as their respective 'original' which are also located in this folder.

This also works on large video files, try the above method of piping the 'Encrypted' file
through the cipher with the give keyfile and you will get its original!

Here is an example of how run the program correctly.

        cat diceEncrypted.jpg | ./encryptUtil  -n 8 -k testKeyfile > diceDecrypted.jpg


Please feel free to test this with any file as the keyfile, just ensure you use the 
same key to DECRYPT that was used to ENCRYPT.

THANK YOU,
Chris
