<h3>Read Me: Subtitle Synchronizer </h3>
Behavior: 
 Currently implemented to work with SubRip (.srt) only.
 
Input:
	filename_pre.srt
	
output: 
	filename.srt

Platforms:
 Windows: Currently tested on Windows, works very well.
 Linux: With some minor modifications code will work on Linux too.

Cmd Example,
 Example delay subtitle to 16 seconds and fix sequence number
 H:> SubSync.exe "D:\TEDTalks\Tan Le - My immigration story (no sub)_2011X-480p_pre.srt" '00:00:16,000' 0
