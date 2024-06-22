SUBSHIFT
========
a small utility for shifting movie subtitles saved in `{start_frame}{end_frame}text` format

usage:
  ./subshift <filepath> <num>

  `filepath` - path to original subtitles  
  `num` - shift value in seconds, assuming default FPS = 23.976

output subtitles are saved in the same location to a new file with ".shifted.srt" suffix

