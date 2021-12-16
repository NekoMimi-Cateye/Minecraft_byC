void Wavelet3d(double*, double);
void PerlinNoize3d(int*, int, int, int, unsigned int);

void Wavelet3d(double *data, double amp)
{
    double x_val, y_val, z_val;
    double dx, dy, dz;
    for(int x=-128; x<128; x++)
    {
        dx = (double)x / 128.0;
        if (x > 0)
            x_val = 1.0 - 3.0 * (dx * dx) + 2.0 * (dx * dx * dx);
        else
            x_val = 1.0 - 3.0 * (dx * dx) - 2.0 * (dx * dx * dx);
        for(int y=-128; y<128; y++)
        {
            dy = (double)y / 128.0;
            if (y > 0)
                y_val = 1.0 - 3.0 * (dy * dy) + 2.0 * (dy * dy * dy);
            else
                y_val = 1.0 - 3.0 * (dy * dy) - 2.0 * (dy * dy * dy);
            for(int z=-128; z<128; z++)
            {
                dz = (double)z / 128.0;
                if (z > 0)
                    z_val = 1.0 - 3.0 * (dz * dz) + 2.0 * (dz * dz * dz);
                else
                    x_val = 1.0 - 3.0 * (dz * dz) - 2.0 * (dz * dz * dz);
                *(data+(x+128)*65536+(y+128)*256+z+128) = x_val * y_val * z_val * amp;
            }
        }
    }
}

void PerlinNoize3d(int* data, int chunk_xpos, int chunk_zpos, int use_seed, unsigned int seed)
{
    int noizeport_x = chunk_xpos / 8;
    int noizeport_z = chunk_zpos / 8;
    int mod_chunk_xpos = chunk_xpos % 8;
    int mod_chunk_zpos = chunk_zpos % 8;

    // require 400MB + a (memory)
    double *C = (double*)malloc(sizeof(double)*256*256*256);        // Wavelet data
    double *W = (double*)malloc(sizeof(double)*128*128*128*8);      // Graph data for marge
    double *Wp0 = (double*)malloc(sizeof(double)*128*128*128*4);    // Graph data (marge z)
    double *Wp1 = (double*)malloc(sizeof(double)*128*128*128*2);    // Graph data (marge z)
    double *Wm = (double*)malloc(sizeof(double)*128*128*128);       // Graph data (marge z)
    double *a = (double*)malloc(sizeof(double)*8*2*8*3);            // Gradient data

    double dx, dy, dz;
    // Wavelet function
    Wavelet3d(C, 1.0);
    // set seed value
    if (use_seed == 1)
        srand(seed);
    else
        srand((unsigned int)time(NULL));
    // set random gradient
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<2; j++)
        {
            for(int k=0; k<8; k++)
            {
                *(a+i*48+j*24+k*3+0) = (double)rand() / RAND_MAX;
                *(a+i*48+j*24+k*3+1) = (double)rand() / RAND_MAX;
                *(a+i*48+j*24+k*3+2) = (double)rand() / RAND_MAX;
            }
        }
    }
    for (int noizeport_y = 0; noizeport_y<2; noizeport_y++)
    {
        for(int x=0; x<128; x++)
        {
            dx = (double)x / 128.0;
            for(int y=0; y<128; y++)
            {
                dy = (double)y / 128.0;
                for(int z=0; z<128; z++)
                {
                    dz = (double)z / 128.0;
                    // Calculate Graphs
                    *(W+x*131072+y*1024+z*8+0) = *(a+(noizeport_x+1)*48+(noizeport_y+1)*24+(noizeport_z+1)*3+0) * dx + *(a+(noizeport_x+1)*48+(noizeport_y+1)*24+(noizeport_z+1)*3+1) * dy + *(a+(noizeport_x+1)*48+(noizeport_y+1)*24+(noizeport_z+1)*3+2) * dz * *(C+(x+128)*65536+(y+128)*256+z+128);
                    *(W+x*131072+y*1024+z*8+1) = *(a+(noizeport_x+1)*48+(noizeport_y+1)*24+(noizeport_z+0)*3+0) * dx + *(a+(noizeport_x+1)*48+(noizeport_y+1)*24+(noizeport_z+0)*3+1) * dy + *(a+(noizeport_x+1)*48+(noizeport_y+1)*24+(noizeport_z+0)*3+2) * dz * *(C+(x+128)*65536+(y+128)*256+z+128);
                    *(W+x*131072+y*1024+z*8+2) = *(a+(noizeport_x+1)*48+(noizeport_y+0)*24+(noizeport_z+1)*3+0) * dx + *(a+(noizeport_x+1)*48+(noizeport_y+0)*24+(noizeport_z+1)*3+1) * dy + *(a+(noizeport_x+1)*48+(noizeport_y+0)*24+(noizeport_z+1)*3+2) * dz * *(C+(x+128)*65536+(y+128)*256+z+128);
                    *(W+x*131072+y*1024+z*8+3) = *(a+(noizeport_x+1)*48+(noizeport_y+0)*24+(noizeport_z+0)*3+0) * dx + *(a+(noizeport_x+1)*48+(noizeport_y+0)*24+(noizeport_z+0)*3+1) * dy + *(a+(noizeport_x+1)*48+(noizeport_y+0)*24+(noizeport_z+0)*3+2) * dz * *(C+(x+128)*65536+(y+128)*256+z+128);
                    *(W+x*131072+y*1024+z*8+4) = *(a+(noizeport_x+0)*48+(noizeport_y+1)*24+(noizeport_z+1)*3+0) * dx + *(a+(noizeport_x+0)*48+(noizeport_y+1)*24+(noizeport_z+1)*3+1) * dy + *(a+(noizeport_x+0)*48+(noizeport_y+1)*24+(noizeport_z+1)*3+2) * dz * *(C+(x+128)*65536+(y+128)*256+z+128);
                    *(W+x*131072+y*1024+z*8+5) = *(a+(noizeport_x+0)*48+(noizeport_y+1)*24+(noizeport_z+0)*3+0) * dx + *(a+(noizeport_x+0)*48+(noizeport_y+1)*24+(noizeport_z+0)*3+1) * dy + *(a+(noizeport_x+0)*48+(noizeport_y+1)*24+(noizeport_z+0)*3+2) * dz * *(C+(x+128)*65536+(y+128)*256+z+128);
                    *(W+x*131072+y*1024+z*8+6) = *(a+(noizeport_x+0)*48+(noizeport_y+0)*24+(noizeport_z+1)*3+0) * dx + *(a+(noizeport_x+0)*48+(noizeport_y+0)*24+(noizeport_z+1)*3+1) * dy + *(a+(noizeport_x+0)*48+(noizeport_y+0)*24+(noizeport_z+1)*3+2) * dz * *(C+(x+128)*65536+(y+128)*256+z+128);
                    *(W+x*131072+y*1024+z*8+7) = *(a+(noizeport_x+0)*48+(noizeport_y+0)*24+(noizeport_z+0)*3+0) * dx + *(a+(noizeport_x+0)*48+(noizeport_y+0)*24+(noizeport_z+0)*3+1) * dy + *(a+(noizeport_x+0)*48+(noizeport_y+0)*24+(noizeport_z+0)*3+2) * dz * *(C+(x+128)*65536+(y+128)*256+z+128);

                    //marge (way: z)
                    *(Wp0+x*65536+y*512+z*4+0) = *(W+x*131072+y*1024+z*8+0) + dz * (*(W+x*131072+y*1024+z*8+1) - *(W+x*131072+y*1024+z*8+0));
                    *(Wp0+x*65536+y*512+z*4+1) = *(W+x*131072+y*1024+z*8+2) + dz * (*(W+x*131072+y*1024+z*8+3) - *(W+x*131072+y*1024+z*8+2));
                    *(Wp0+x*65536+y*512+z*4+2) = *(W+x*131072+y*1024+z*8+4) + dz * (*(W+x*131072+y*1024+z*8+5) - *(W+x*131072+y*1024+z*8+4));
                    *(Wp0+x*65536+y*512+z*4+3) = *(W+x*131072+y*1024+z*8+6) + dz * (*(W+x*131072+y*1024+z*8+7) - *(W+x*131072+y*1024+z*8+6));

                    //marge (way: y)
                    *(Wp1+x*32768+y*256+z*2+0) = *(Wp0+x*65536+y*512+z*4+0) + dy * (*(Wp0+x*65536+y*512+z*4+1) - *(Wp0+x*65526+y*512+z*4+0));
                    *(Wp1+x*32768+y*256+z*2+1) = *(Wp0+x*65536+y*512+z*4+2) + dy * (*(Wp0+x*65536+y*512+z*4+3) - *(Wp0+x*65526+y*512+z*4+2));
                }
            }
        }
        int xp, yp, zp;
        for(int x=0; x<16; x++)
        {
            xp = 16 * mod_chunk_xpos + x;
            dx = (double)(xp) / 128.0;
            for(int y=0; y<128; y++)
            {
                yp = 128 * noizeport_y + y;
                for(int z=0; z<16; z++)
                {
                    zp = 16 * mod_chunk_zpos + z;
                    //marge (way: x)
                    *(Wm+x*16384+y*128+z) = *(Wp1+xp*32768+y*256+zp*2+0) + dx * (*(Wp1+xp*32768+y*256+zp*2+1) - *(Wp1+xp*32768+y*256+zp*2+0));
                    if (*(Wm+x*16384+y*128+z) > 0)
                        *(data+x*4096+yp*16+z) = 1;
                    else
                        *(data+x*4096+yp*16+z) = 0;
                }
            }
        }
    }
    free(C);
    free(W);
    free(Wp0);
    free(Wp1);
    free(Wm);
    free(a);
}

void CreateWorld()
{
    int* onechunk3d;
    int* onechunk2d;
    double* dia_rate;
    double* onechunk1d_x;
    double* onechunk1d_y;
    double* onechunk1d_z;
    double rate_rate;

    onechunk3d = (int*)malloc(sizeof(int)*16*256*16);
    onechunk2d = (int*)malloc(sizeof(int)*17*16);
    onechunk1d_x = (double*)malloc(sizeof(double)*16);
    onechunk1d_y = (double*)malloc(sizeof(double)*16*16);
    onechunk1d_z = (double*)malloc(sizeof(double)*16);
    dia_rate = (double*)malloc(sizeof(double)*64*64*16*16*16);
    for (int i=0; i<WORLD_CHUNK; i++)
    {
        for (int j=0; j<WORLD_CHUNK; j++)
        {
            PerlinNoize2d(onechunk2d, i, j, 1, SEED_NUM);
            PerlinNoize3d(onechunk3d, i, j, 1, SEED_NUM);
            for (int x=0; x<16; x++)
            {
                for (int z=0; z<16; z++)
                {
                    for (int y=0; y<256; y++)
                    {
                        if (y >= *(onechunk2d+16*x+z))
                            *(onechunk3d+x*4096+y*16+z) = 0;
                        else if(*(onechunk3d+x*4096+y*16+z) == 1)
                        {
                            if (y < *(onechunk2d+16*x+z)-3)
                            {
                                *(onechunk3d+x*4096+y*16+z) = 2;
                            }
                            if (y < 16)
                            {
                                if (y >= 5 || y <= 12)
                                    rate_rate = 1.0;
                                else if (y < 5)
                                    rate_rate = (1.0 - (double)(y) / 5.0);
                                else if (y > 12)
                                    rate_rate = (double)(15 - y) / 3.0;
                                if (*(dia_rate+262144*i+4096*j+256*x+16*y+z) <= (diamond_rate * rate_rate))
                                    *(onechunk3d+x*4096+y*16+z) = 3;
                            }
                        }
                        if (y == 0)
                        {
                            *(onechunk3d+x*4096+y*16+z) = 4;
                        }
                    }
                }
            }
            SaveChunk(onechunk3d, i, j);
            printf("\rcreated chunk: %d ,%d (%5.2lf%%)", i+1, j+1, (double)(64*i+j)/(double)(64*64));
            fflush(stdout);
        }
    }
    free(onechunk3d);
    free(onechunk2d);
    free(onechunk1d_x);
    free(onechunk1d_y);
    free(onechunk1d_z);
}
