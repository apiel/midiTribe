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

function getOutput() {
    const { value: key } = document.getElementById('output');
    return midi.outputs.get(key);
}

document.getElementById('channel').onchange = ({ target: { value } }) => {
    sendSustain(value, false);
};

document.querySelectorAll('#pad button, #keyboard button').forEach((bt) => {
    const { channel, note } = bt.dataset;

    // 0x90 = 144 note on ch1
    bt.onmousedown = () =>
        getOutput().send([143 + Number(channel), Number(note), 100]);
    // 0x80 = 128 note of ch1
    bt.onmouseup = () =>
        getOutput().send([127 + Number(channel), Number(note), 100]);
});

document.querySelectorAll('#mode button').forEach((bt) => {
    const { note } = bt.dataset;

    // 0xB0 = 176 note on ch1
    bt.onclick = () => sendSustain(note);
});

document.querySelectorAll('#pot input').forEach((slide) => {
    const { control } = slide.dataset;

    slide.oninput = ({ target: { value } }) => {
        getOutput().send([0xB0, Number(control), Number(value)]);
    };
});

function sendSustain(note, pad = true) {
    getOutput().send([0xb0, 64, 127]);
    // 0x90 = 144 note on ch1 + 9 for ch10
    getOutput().send([pad ? 153 : 144, Number(note), 100]);
    getOutput().send([0xb0, 64, 0]);
}
