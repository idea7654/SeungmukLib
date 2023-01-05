const fs = require('fs');
const exec = require('child_process').exec;
const path = require('path');

function loadFbs() {
    const flatcPath = path.resolve(__dirname, '../SeungmukLib', 'Packet');
    exec('flatc --cpp PacketDefine.fbs', { cwd: flatcPath }, (err, data) => {
        if (err) console.log(err);
        else console.log('Fbs Generate Success!');
    });
}
loadFbs();

let defaultText = `#pragma once \n#include "pch.h" \nusing namespace std;\nstatic flatbuffers::FlatBufferBuilder builder(1024);\n`;

let fileText = fs.readFileSync('../SeungmukLib/Packet/PacketDefine.fbs', 'utf8');
fileText = fileText.split('\n');

const packetList = [];

const packetStruct = function () {
    let className;
    let parameter = [];
};

let startFlag = false;
let packetListIndex = -1;
fileText.forEach((element) => {
    if (element.substr(0, 5) == 'table') {
        const className = element.substring(6, element.length - 1);
        if (className != `Message`) {
            let packet = new packetStruct();
            packet.className = className;
            packet.parameter = [];
            packetList.push(packet);
            startFlag = true;
            packetListIndex++;
        }
        return;
    }

    if (startFlag) {
        if (element[0] == '}') {
            startFlag = false;
        }

        if (element[0] != '{' && element[0] != '}') {
            const centerString = element.indexOf(':');
            const endString = element.indexOf(';');

            const type = element.substring(centerString + 2, endString);
            const parameterName = element.substr(1, centerString - 1);

            const text = type + parameterName + ';';

            if (parameterName != 'packet') {
                packetList[packetListIndex].parameter.push(text);
            }
        }
    }
});

packetList.forEach((element) => {
    let classText = `
class Packet_${element.className} : public Packet
{
`;

    element.parameter.forEach((text) => {
        classText += `    ${text}\n`;
    });

    classText += `
public:
    Packet_${element.className}()
    {
        SetPacketType(MESSAGE_ID::MESSAGE_ID_${element.className});  
    }
`;

    //Encode
    classText += `
    virtual uint8_t* Encode(int& packetLength) override
    {
`;
    element.parameter.forEach((text) => {
        const name = text.split(' ')[1].replace(';', '');
        const type = text.split(' ')[0];

        if (type == 'string') {
            classText += `        auto ${name}String = builder.CreateString(${name});`;
        }
    });

    classText += `
        auto makePacket = Create${element.className}(builder`;
    element.parameter.forEach((text) => {
        const name = text.split(' ')[1].replace(';', '');
        const type = text.split(' ')[0];

        if (type == 'string') {
            classText += `, ${name}String`;
        } else {
            classText += `, ${name}`;
        }
    });
    classText += `);`;

    classText += `
        auto packet = CreateMessage(builder, MESSAGE_ID::MESSAGE_ID_${element.className}, makePacket.Union());
        builder.Finish(packet);
        packetLength = builder.GetSize();

        auto data = builder.GetBufferPointer();
        builder.Clear();
        return data;
    }
`;
    //Encode End

    //Decode
    classText += `
    virtual void Decode(const Message *packet) override
    {
        auto data = static_cast<const ${element.className}*>(packet->packet());
`;
    element.parameter.forEach((text) => {
        const name = text.split(' ')[1].replace(';', '');
        const type = text.split(' ')[0];

        if (type == 'string') {
            classText += `
        ${name} = data->${name}()->c_str();`;
        } else {
            classText += `
        ${name} = data->${name}();`;
        }
    });
    classText += `
    }
`;
    //Decode End

    classText += '};\n';

    defaultText += classText;
});

fs.writeFileSync('../SeungmukLib/ContentsPacket.hpp', defaultText);

console.log('Finish Process!');
