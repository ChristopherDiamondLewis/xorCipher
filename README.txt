Hello!

This is an pretty neat multithreaded XOR cipher that I wrote, where the key used for the cipher is rotated X number of times, where
X is the number of bits in the key file, so if the file is 8 bytes the key will repeat every 64 rotations, which is pretty cool.


While this program is multithreaded, you will notice that the main bottleneck of the program is not how many threads you use but how large of a 
key you use.

Here are some simple instructions for my cipher and testing the files included.

If you run the makefile or the compile command you can pipe the files named 
'appleEncrypted.jpg' and 'diceEncrypted.jpg' while using the keyfile provided and redirecting 
output to  any .jpg (or whatever the correct extension of the original file is). 
You will see the end result of 'appleEncrypted.jpg' and 'diceEncrypted.jpg' 
being the same as their respective 'original' which are also located in this folder.

This also works on large video files, try the above method of piping the 'Encrypted' file
through the cipher with the given keyfile and you will get its original!

Here is an example of how run the program correctly using the files provided, of course you could use any files you want 
as well as any 'keyfile' you want.

        cat diceEncrypted.jpg | ./encryptUtil  -n NUMBER_OF_THREADS -k testKeyfile > diceDecrypted.jpg


Please feel free to test this with any file as the keyfile, just ensure you use the 
same key to DECRYPT that was used to ENCRYPT.

THANK YOU,
Chris
