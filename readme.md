Presentation: https://docs.google.com/presentation/d/1RpzPVmM1bkLmd_SVYJ0rYbmGC7tMXw5Krsl7O5ntq38/edit?usp=sharing
## docker cluster setup
- docker compose up 
- docker exec -it mpi-master bash
    - ssh-keyscan mpi-master >> /root/.ssh/known_hosts
    - ssh-keyscan mpi-worker1 >> /root/.ssh/known_hosts
    - ssh-keyscan mpi-worker2 >> /root/.ssh/known_hosts
    - mpirun -np 3 -H mpi-master:1,mpi-worker1:1,mpi-worker2:1 ./psb_algorithm
    