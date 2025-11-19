// ==============================================================================
// DYNAMIXEL CONTROLLER IMPLEMENTATION
// ==============================================================================

#include "dynamixel_controller.h"
#include <cstdio>
#include <cmath>

// Global handlers
dynamixel::PortHandler* portHandler = nullptr;
dynamixel::PacketHandler* packetHandler = nullptr;
dynamixel::GroupSyncWrite* groupSyncWriteCurr = nullptr;
dynamixel::GroupSyncRead* groupSyncRead = nullptr;

bool initializeDynamixel() {
    portHandler = dynamixel::PortHandler::getPortHandler(DEVICENAME);
    packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL_VERSION);
    
    if (!portHandler->openPort()) {
        printf("Failed to open the port!\n");
        return false;
    }
    printf("✓ Port opened successfully\n");
    
    if (!portHandler->setBaudRate(BAUDRATE)) {
        printf("Failed to set baudrate!\n");
        return false;
    }
    printf("✓ Baudrate set to %d\n", BAUDRATE);
    
    // Initialize GroupSyncWrite for Goal Current
    groupSyncWriteCurr = new dynamixel::GroupSyncWrite(portHandler, packetHandler, ADDR_GOAL_CURRENT, LEN_GOAL_CURRENT);
    
    // Initialize GroupSyncRead for Position, Velocity, Current
    groupSyncRead = new dynamixel::GroupSyncRead(portHandler, packetHandler, ADDR_PRESENT_CURRENT, TOTAL_LEN_READ);
    
    // Add motors to SyncRead
    groupSyncRead->addParam(DXL1_ID);
    groupSyncRead->addParam(DXL2_ID);
    groupSyncRead->addParam(DXL3_ID);
    groupSyncRead->addParam(DXL4_ID);
    
    return true;
}

dynamixel::PortHandler* getPortHandler() {
    return portHandler;
}

dynamixel::PacketHandler* getPacketHandler() {
    return packetHandler;
}

void configureMotor(uint8_t id) {
    uint8_t dxl_error = 0;
    int dxl_comm_result;
    
    // Set operating mode to Current Control Mode (0)
    // Note: Check your motor manual. Usually 0 is Current Control.
    dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, id, ADDR_OPERATING_MODE, 0, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS) {
        printf("✗ Motor %d: Failed to set Current Mode\n", id);
    } else {
        printf("✓ Motor %d: Current Mode set\n", id);
    }
    
    // Set return delay to 0
    packetHandler->write1ByteTxRx(portHandler, id, ADDR_RETURN_DELAY, 0, &dxl_error);
    
    // Set current limit
    packetHandler->write2ByteTxRx(portHandler, id, ADDR_CURRENT_LIMIT, CURR_PEAK, &dxl_error);
    
    // Enable Torque
    packetHandler->write1ByteTxRx(portHandler, id, ADDR_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);
}

void disableAllMotors() {
    uint8_t dxl_error = 0;
    
    if (packetHandler && portHandler) {
        packetHandler->write1ByteTxRx(portHandler, DXL1_ID, ADDR_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
        packetHandler->write1ByteTxRx(portHandler, DXL2_ID, ADDR_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
        packetHandler->write1ByteTxRx(portHandler, DXL3_ID, ADDR_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
        packetHandler->write1ByteTxRx(portHandler, DXL4_ID, ADDR_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
        
        // Also disable gripper
        packetHandler->write1ByteTxRx(portHandler, 15, ADDR_TORQUE_ENABLE, TORQUE_DISABLE, &dxl_error);
        
        portHandler->closePort();
    }
    
    printf("✓ All motors disabled\n");
}

void readMotorStates(int32_t* pos, int32_t* vel, int16_t* cur) {
    int dxl_comm_result = groupSyncRead->txRxPacket();
    if (dxl_comm_result != COMM_SUCCESS) {
        // printf("%s\n", packetHandler->getTxRxResult(dxl_comm_result));
        return;
    }
    
    // Check if data is available
    if (groupSyncRead->isAvailable(DXL1_ID, ADDR_PRESENT_CURRENT, TOTAL_LEN_READ)) {
        cur[0] = (int16_t)groupSyncRead->getData(DXL1_ID, ADDR_PRESENT_CURRENT, LEN_PRESENT_CURRENT);
        vel[0] = (int32_t)groupSyncRead->getData(DXL1_ID, ADDR_PRESENT_VELOCITY, LEN_PRESENT_VELOCITY);
        pos[0] = (int32_t)groupSyncRead->getData(DXL1_ID, ADDR_PRESENT_POSITION, LEN_PRESENT_POSITION);
    }
    
    if (groupSyncRead->isAvailable(DXL2_ID, ADDR_PRESENT_CURRENT, TOTAL_LEN_READ)) {
        cur[1] = (int16_t)groupSyncRead->getData(DXL2_ID, ADDR_PRESENT_CURRENT, LEN_PRESENT_CURRENT);
        vel[1] = (int32_t)groupSyncRead->getData(DXL2_ID, ADDR_PRESENT_VELOCITY, LEN_PRESENT_VELOCITY);
        pos[1] = (int32_t)groupSyncRead->getData(DXL2_ID, ADDR_PRESENT_POSITION, LEN_PRESENT_POSITION);
    }
    
    if (groupSyncRead->isAvailable(DXL3_ID, ADDR_PRESENT_CURRENT, TOTAL_LEN_READ)) {
        cur[2] = (int16_t)groupSyncRead->getData(DXL3_ID, ADDR_PRESENT_CURRENT, LEN_PRESENT_CURRENT);
        vel[2] = (int32_t)groupSyncRead->getData(DXL3_ID, ADDR_PRESENT_VELOCITY, LEN_PRESENT_VELOCITY);
        pos[2] = (int32_t)groupSyncRead->getData(DXL3_ID, ADDR_PRESENT_POSITION, LEN_PRESENT_POSITION);
    }
    
    if (groupSyncRead->isAvailable(DXL4_ID, ADDR_PRESENT_CURRENT, TOTAL_LEN_READ)) {
        cur[3] = (int16_t)groupSyncRead->getData(DXL4_ID, ADDR_PRESENT_CURRENT, LEN_PRESENT_CURRENT);
        vel[3] = (int32_t)groupSyncRead->getData(DXL4_ID, ADDR_PRESENT_VELOCITY, LEN_PRESENT_VELOCITY);
        pos[3] = (int32_t)groupSyncRead->getData(DXL4_ID, ADDR_PRESENT_POSITION, LEN_PRESENT_POSITION);
    }
}

void convertToJointSpace(const int32_t* pos_ticks, const int32_t* vel_ticks, 
                         Eigen::Vector4d& q, Eigen::Vector4d& dq) {
    // Constants for conversion
    const double TICKS_TO_RAD = (2.0 * M_PI) / 4095.0;
    const double VEL_TO_RAD_S = 0.229 * (2.0 * M_PI) / 60.0; // 0.229 rpm per unit
    
    // Zero offsets (ticks) - Adjust these based on your calibration
    const int32_t offsets[4] = {2048, 2048, 2048, 2048};
    
    for (int i = 0; i < 4; i++) {
        q(i) = (pos_ticks[i] - offsets[i]) * TICKS_TO_RAD;
        dq(i) = vel_ticks[i] * VEL_TO_RAD_S;
    }
}

void sendCurrentCommands(int16_t curr_d1, int16_t curr_d2, 
                         int16_t curr_d3, int16_t curr_d4) {
    
    uint8_t param1[2] = {(uint8_t)(curr_d1 & 0xFF), (uint8_t)((curr_d1 >> 8) & 0xFF)};
    uint8_t param2[2] = {(uint8_t)(curr_d2 & 0xFF), (uint8_t)((curr_d2 >> 8) & 0xFF)};
    uint8_t param3[2] = {(uint8_t)(curr_d3 & 0xFF), (uint8_t)((curr_d3 >> 8) & 0xFF)};
    uint8_t param4[2] = {(uint8_t)(curr_d4 & 0xFF), (uint8_t)((curr_d4 >> 8) & 0xFF)};
    
    groupSyncWriteCurr->addParam(DXL1_ID, param1);
    groupSyncWriteCurr->addParam(DXL2_ID, param2);
    groupSyncWriteCurr->addParam(DXL3_ID, param3);
    groupSyncWriteCurr->addParam(DXL4_ID, param4);
    groupSyncWriteCurr->txPacket();
    groupSyncWriteCurr->clearParam();
}
