Short Term Storage Archetype for Cyclus
---------------------------------------
This provides a proof-of-concept for short term storage for cyclus.

In the **direct** case, there is a different commodity for each spent fuel
such that wet storage may only supply dry storage, and dry storage may only
supply the deep geologic repository.

In the **open** case, there is a single spent fuel commodity and any
facility may trade with any other facility. However, the DRE greedy solver
performs oddly in this case due to averaging preferences. See CEP26 for
more discussion.