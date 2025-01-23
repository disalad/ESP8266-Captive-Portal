document.addEventListener('DOMContentLoaded', () => {
    const inputs = document.querySelectorAll('.input');

    inputs.forEach(input => {
        input.addEventListener('input', () => {
            const label = input.nextElementSibling;
            if (input.value) {
                label.classList.add('label-top');
            } else {
                label.classList.remove('label-top');
            }
        });

        input.addEventListener('blur', () => {
            const label = input.nextElementSibling;
            if (!input.value) {
                label.classList.remove('label-top');
            }
        });

        input.addEventListener('focus', () => {
            const label = input.nextElementSibling;
            if (input.value) {
                label.classList.add('label-top');
            }
        });
    });
});
