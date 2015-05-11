
/**
 * @note Test app flags as following
 * 
 * Testcase flags: everyone just adding; adding and removing
 * Implementation fine grained lock (FGL); optimistic sync (OS); lazy synchronization (LS), list base-set (LBS), reference implementation REF
 * @var[in] -i the implementation to be used {FGL, OS, LS, LBS, REF}
 * @var[in] -c thread count to be used [1, 512]
 * @var[in] -r counter of repeatedly serialized test execution
 */
./ampp -i<implementaion> -c<threadCount> -r<repetitionCycles> --add --remove --random


/* to be tested */
- how long Inserting
- how fair is it 
 -- e.g. insert: every thread inserts 30sec, how is the count
 -- e.g. insert/remove: every threads inserts one, removes one how often did every thread that
 -- e.g. random add/remove with seed
- throughput
 -- due to fair
- latency
 -- add and or remove

/* Interface */
- insert
- erase
- find/contains
