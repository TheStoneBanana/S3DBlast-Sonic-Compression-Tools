# Sonic 3D Blast Sonic Compression Tools

Here is a compressor and decompressor for the compression format used for Sonic's graphics in Sonic 3D Blast. This came about after hearing, through a tweet, that GameHut lost the original tools to compress to this format. Knowing this, I took a dive into Sonic 3D Blast and quickly figured out a format, writing these tools within the next day.

## Locating Compressed Data

Go to 0x100000 in a final Sonic 3D Blast ROM.

![alt text](https://i.imgur.com/tvfPvtn.png)

This is a table of entries for the compressed data for each of Sonic's frames. Pick an entry from the list, ignoring the first byte (this is information you don't need), and go to that address. I'll be using the first entry at 0x4D0 for example purposes.

At that address, look three bytes back, and add that value to the original address. This will take you to the beginning of the compressed data for a frame.

![alt text](https://i.imgur.com/V7ziGwn.png)

Here I have highlighted the entirety of the compressed data, which you can extract and put into my tool. This is the result:

![alt text](https://i.imgur.com/EMaHnQn.png)

Neat right?

## Closing Thoughts

Enjoy my messy code I suppose. If you find any problems, please let me know and I'll fix them ASAP.