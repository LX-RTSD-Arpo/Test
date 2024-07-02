#include "./lib/modbus.h"

#define SLAVE_ID 42
#define SERIAL_PORT "/dev/ttyUSB0"

int main()
{
    modbus_t *ctx;

    ctx = modbus_new_rtu(SERIAL_PORT, 9600, 'N', 8, 1);
    if (ctx == NULL)
    {
        fprintf(stderr, "Unable to create Modbus context\n");
        return 1;
    }

    modbus_set_slave(ctx, SLAVE_ID);
    if (modbus_connect(ctx) == -1)
    {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return 1;
    }

    while (1)
    {
        int start_address = 100;
        uint16_t data[num_registers];
        int num_registers = 10;
        uint16_t data[num_registers];

        int rc = modbus_read_registers(ctx, start_address, num_registers, data);

        printf("Read data: ");
        if (rc != num_registers)
        {
            fprintf(stderr, "Failed to read data: %s\n", modbus_strerror(errno));
        }
        else
        {
            printf("Read data from slave %d: ", SLAVE_ID);
            for (int i = 0; i < num_registers; i++)
            {
                printf("%d ", data[i]);
            }
            printf("\n");
        }

        // Delay before the next read (adjust as needed)
        sleep(1);
    }
    modbus_close(ctx);
    modbus_free(ctx);
}
}

/*
void ReadRegister()
{
    int start_address = 100;
    int num_registers = 10;
    uint16_t *data = (uint16_t *)malloc(num_registers * sizeof(uint16_t));
    int rc = modbus_read_registers(ctx, start_address, num_registers, data);
    if (rc == num_registers)
    {
        printf("Read data: ");
        for (int i = 0; i < num_registers; i++)
        {
            printf("%d ", data[i]);
        }
        printf("\n");
    }
    else
    {
        printf("Failed to read data\n");
    }
    free(data);
}

void WriteRegister()
{
    int start_address = 200;
    int num_registers = 4;
    uint16_t data[] = {10, 20, 30, 40};
    int rc = modbus_write_registers(ctx, start_address, num_registers, data);
    if (rc == num_registers)
    {
        printf("Data written successfully.\n");
    }
    else
    {
        printf("Failed to write data\n");
    }
}
*/