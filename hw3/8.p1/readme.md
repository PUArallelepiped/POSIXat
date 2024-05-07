# Programming Project for Ch.8*: Banker’s Algorithm
> run
```bash
gcc banker.c ; ./a.out 10 5 7 8
```

## result 
```
Available: 10 5 7 8 ;Allocation: 0 0 0 0 ;Need: 6 4 7 3 ;Customer 0 requesting: 1 1 1 3 
Customer 0 request granted
Available: 9 4 6 5 ;Allocation: 0 0 0 0 ;Need: 2 5 3 3 ;Customer 2 requesting: 0 0 0 0 
Customer 2 request granted
Available: 9 4 6 5 ;Allocation: 0 0 0 0 ;Need: 2 5 3 3 ;Customer 2 requesting: 2 2 3 1 
Customer 2 request denied
Available: 9 4 6 5 ;Allocation: 0 0 0 0 ;Need: 2 5 3 3 ;Customer 2 requesting: 2 2 2 3 
Customer 2 request denied
Available: 9 4 6 5 ;Allocation: 0 0 0 0 ;Need: 2 5 3 3 ;Customer 2 requesting: 1 1 1 2 
Customer 2 request denied
Available: 9 4 6 5 ;Allocation: 0 0 0 0 ;Need: 2 5 3 3 ;Customer 2 requesting: 2 5 3 3 
Customer 2 request denied
Available: 9 4 6 5 ;Allocation: 0 0 0 0 ;Need: 2 5 3 3 ;Customer 2 requesting: 2 5 3 3 
Customer 2 request denied
Available: 9 4 6 5 ;Allocation: 0 0 0 0 ;Need: 2 5 3 3 ;Customer 2 requesting: 2 5 3 3 
Customer 2 request denied
Available: 9 4 6 5 ;Allocation: 0 0 0 0 ;Need: 2 5 3 3 ;Customer 2 requesting: 2 5 3 3 
Customer 2 request denied
Available: 9 4 6 5 ;Allocation: 0 0 0 0 ;Need: 2 5 3 3 ;Customer 2 requesting: 2 5 3 3 
Customer 2 request denied
Available: 9 4 6 5 ;Allocation: 0 0 0 0 ;Need: 2 5 3 3 ;Customer 2 requesting: 2 5 3 3 
Customer 2 request denied
Available: 9 4 6 5 ;Allocation: 0 0 0 0 ;Need: 2 5 3 3 ;Customer 2 requesting: 2 5 3 3 
Customer 2 request denied
Available: 9 4 6 5 ;Allocation: 0 0 0 0 ;Need: 2 5 3 3 ;Customer 2 requesting: 2 5 3 3 
Customer 2 request denied
Available: 9 4 6 5 ;Allocation: 0 0 0 0 ;Need: 2 5 3 3 ;Customer 2 requesting: 2 5 3 3 
Customer 2 request denied
Available: 9 4 6 5 ;Allocation: 0 0 0 0 ;Need: 2 5 3 3 ;Customer 2 requesting: 2 5 3 3 
Customer 2 request denied
Available: 9 4 6 5 ;Allocation: 0 0 0 0 ;Need: 4 2 3 2 ;Customer 1 requesting: 4 1 2 1 
Customer 1 request granted
Customer 1 releasing: 2 0 2 1 
Available: 7 3 6 5 ;Allocation: 2 1 0 0 ;Need: 2 1 3 2 ;Customer 1 requesting: 1 0 3 1 
Customer 1 request granted
Customer 0 releasing: 1 1 1 3 
Available: 7 4 4 7 ;Allocation: 0 0 0 0 ;Need: 2 5 3 3 ;Customer 2 requesting: 2 5 3 3 
Customer 2 request denied
Available: 7 4 4 7 ;Allocation: 0 0 0 0 ;Need: 2 5 3 3 ;Customer 2 requesting: 2 5 3 3 
Customer 2 request denied
Customer 1 releasing: 3 0 1 0 
Available: 10 4 5 7 ;Allocation: 0 1 2 1 ;Need: 4 1 1 1 ;Customer 1 requesting: 1 0 0 1 
Customer 1 request granted
Available: 9 4 5 6 ;Allocation: 0 0 0 0 ;Need: 5 5 7 5 ;Customer 4 requesting: 2 1 3 4 
Customer 4 request granted
Customer 4 releasing: 2 0 0 1 
Available: 9 3 2 3 ;Allocation: 0 0 0 0 ;Need: 2 5 3 3 ;Customer 2 requesting: 2 5 3 3 
Customer 2 request denied
Available: 9 3 2 3 ;Allocation: 0 0 0 0 ;Need: 6 4 7 3 ;Customer 0 requesting: 6 2 6 0 
Customer 0 request denied
Customer 1 releasing: 0 1 1 0 
Available: 9 4 3 3 ;Allocation: 0 0 0 0 ;Need: 6 4 7 3 ;Customer 0 requesting: 3 2 4 3 
Customer 0 request denied
Available: 9 4 3 3 ;Allocation: 0 0 0 0 ;Need: 6 4 7 3 ;Customer 0 requesting: 4 4 3 2 
Customer 0 request denied
Available: 9 4 3 3 ;Allocation: 0 0 0 0 ;Need: 2 5 3 3 ;Customer 2 requesting: 2 5 3 3 
Customer 2 request denied
Available: 9 4 3 3 ;Allocation: 0 1 3 3 ;Need: 5 4 4 2 ;Customer 4 requesting: 3 0 4 0 
Customer 4 request denied
Customer 4 releasing: 0 0 0 2 
Available: 9 4 3 5 ;Allocation: 0 1 3 1 ;Need: 5 4 4 4 ;Customer 4 requesting: 2 4 3 1 
Customer 4 request denied
Customer 4 releasing: 0 1 0 0 
Available: 9 5 3 5 ;Allocation: 0 0 3 1 ;Need: 5 5 4 4 ;Customer 4 requesting: 2 0 4 2 
Customer 4 request denied
Customer 4 releasing: 0 0 3 1 
Available: 9 5 6 6 ;Allocation: 0 0 0 0 ;Need: 5 5 7 5 ;Customer 4 requesting: 5 5 7 5 
Customer 4 request denied
Available: 9 5 6 6 ;Allocation: 1 0 1 2 ;Need: 3 2 2 0 ;Customer 1 requesting: 2 0 2 0 
Customer 1 request granted
Customer 1 releasing: 3 0 3 2 
Available: 10 5 7 8 ;Allocation: 0 0 0 0 ;Need: 6 4 7 3 ;Customer 0 requesting: 6 4 7 3 
Customer 0 request granted
Customer 0 releasing: 6 4 7 3 
Customer 0 finished
Available: 10 5 7 8 ;Allocation: 0 0 0 0 ;Need: 5 5 7 5 ;Customer 4 requesting: 5 5 7 5 
Customer 4 request granted
Customer 4 releasing: 5 5 7 5 
Customer 4 finished
Available: 10 5 7 8 ;Allocation: 0 0 0 0 ;Need: 2 5 3 3 ;Customer 2 requesting: 2 5 3 3 
Customer 2 request granted
Customer 2 releasing: 2 5 3 3 
Customer 2 finished
Available: 10 5 7 8 ;Allocation: 0 0 0 0 ;Need: 4 2 3 2 ;Customer 1 requesting: 4 2 3 2 
Customer 1 request granted
Customer 1 releasing: 4 2 3 2 
Customer 1 finished
Available: 10 5 7 8 ;Allocation: 0 0 0 0 ;Need: 6 3 3 2 ;Customer 3 requesting: 1 3 2 0 
Customer 3 request granted
Customer 3 releasing: 0 2 0 0 
Available: 9 4 5 8 ;Allocation: 1 1 2 0 ;Need: 5 2 1 2 ;Customer 3 requesting: 3 1 0 1 
Customer 3 request granted
Customer 3 releasing: 2 0 2 0 
Available: 8 3 7 7 ;Allocation: 2 2 0 1 ;Need: 4 1 3 1 ;Customer 3 requesting: 2 0 0 0 
Customer 3 request granted
Customer 3 releasing: 1 2 0 1 
Available: 7 5 7 8 ;Allocation: 3 0 0 0 ;Need: 3 3 3 2 ;Customer 3 requesting: 1 1 0 2 
Customer 3 request granted
Customer 3 releasing: 4 1 0 2 
Available: 10 5 7 8 ;Allocation: 0 0 0 0 ;Need: 6 3 3 2 ;Customer 3 requesting: 6 3 3 2 
Customer 3 request granted
Customer 3 releasing: 6 3 3 2 
Customer 3 finished
```
