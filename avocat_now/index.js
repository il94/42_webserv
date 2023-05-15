const inputs = [...document.getElementsByTagName('input')];

const resetPlaceholder = (input) => {
    input.placeholder = input.defaultPlaceholder;
};

const removePlaceholder = (input) => {
    if(input.innerHTML === ''){
        input.defaultPlaceholder = input.placeholder;
        input.placeholder = '';
    }
};

inputs.forEach(input => {
    input.onfocus = () => removePlaceholder(input);
    input.onblur = () => resetPlaceholder(input);
});

