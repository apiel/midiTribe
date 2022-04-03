// request MIDI access
if (navigator.requestMIDIAccess) {
    navigator
        .requestMIDIAccess({
            sysex: true,
        })
        .then(onMIDISuccess, onMIDIFailure);
} else {
    alert('No MIDI support in your browser.');
}

let midi;
let selectedInput = -1;

function onMIDISuccess(midiAccess) {
    midi = midiAccess;
    midiAccess.outputs.forEach((midiOutput, key) => {
        console.log('midiOutput', midiOutput.name, midiOutput);
        document.getElementById(
            'output',
        ).innerHTML += `<option value="${key}" ${
            midiOutput.name.includes('Teensy') && 'selected'
        }>${midiOutput.name}</option>`;
    });
}

function onMIDIFailure(error) {
    console.error(
        "No access to MIDI devices or your browser doesn't support WebMIDI API. Please use WebMIDIAPIShim ",
        error,
    );
}

// document.getElementById('note').innerHTML = Array(255)
//     .fill()
//     .map(
//         (_, key) =>
//             `<option value="${(key + 24) % 255}">${(key + 24) % 255}</option>`,
//     )
//     .join('');

// document.getElementById('send').onclick = () => {
//     const { value: data } = document.getElementById('data');
//     const { value: key } = document.getElementById('output');
//     const output = midi.outputs.get(key);
//     const msg = JSON.parse(data);
//     console.log(`Send to ${output.name}:`, msg);
//     output.send(msg);
// };

function getOutput() {
    const { value: key } = document.getElementById('output');
    return midi.outputs.get(key);
}

document.querySelectorAll('#pad button, #keyboard button').forEach((bt) => {
    const { channel, note } = bt.dataset;

    // 0x90 = 144 note on ch1
    bt.onmousedown = () =>
        getOutput().send([143 + Number(channel), Number(note), 100]);
    // 0x80 = 128 note of ch1
    bt.onmouseup = () =>
        getOutput().send([127 + Number(channel), Number(note), 100]);
});
