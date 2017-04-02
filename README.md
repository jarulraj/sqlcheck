Machine
-------

Machine model that emulates a multi-tier storage hierarchy   

# Parameters

sequential and random accesses separate parameters  
read and write separate parameters  

cost for reading from DRAM  
cost for writing to DRAM  

cost for reading directly from NVM  
cost for writing directly to NVM  
cost for reading from NVM to DRAM  
cost for writing from DRAM to NVM  

cost for reading from SSD to DRAM  
cost for writing from DRAM to SSD  
cost for reading from SSD to NVM  
cost for writing from NVM to SSD  

cost for reading from HDD to DRAM  
cost for writing from DRAM to HDD  
cost for reading from HDD to NVM  
cost for writing from NVM to HDD  

number of pages that fit in DRAM  
number of pages that fit in NVM  
number of pages that fit in SSD  
number of pages that fit in HDD  

# Durable pages: 

Must be written back to NVM/SSD/HDD  

Table pages     
Index pages  
Log pages  
Checkpoint pages  

# Ephemeral pages: 

Need not be written back to durable storage  

Temporary table pages  
Temporary memory allocations (logical tiles etc.)  

# Init

Store ephemeral pages on DRAM  
Store durable pages on both DRAM and NVM (copy in both DRAM and NVM)  

SyncDurablePage()  

Evict some durable pages from DRAM to NVM  
EvictDurablePage() // No longer a cooy in DRAM  

NVM is full =>   
EvictDurablePage() // No longer a cooy in DRAM or NVM (stored in SSD)  

SSD is full =>  
EvictDurablePage() // No longer a cooy in DRAM or NVM (stored in HDD)  

# Read a page

In DRAM
On NVM: can either directly read/write or copy it over to DRAM depending on frequency  
On SSD/HDD: must copy it over to DRAM or NVM depending on frequency  

# Write a page

Must exist in DRAM (ephemeral)  
Must write to DRAM and durable storage (durable) (pick NVM/SSD/HDD based on frequency)  

Minimize total time to complete operations

# Modules

LRU, LFU, and ARC caching algorithms  
Multiple caches  
Workload generator  
Parameter definitions   


