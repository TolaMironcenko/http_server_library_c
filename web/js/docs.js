const getbuttonmessage = document.querySelector('.getpointbutton.message')
const point = document.querySelector('.point.message')

getbuttonmessage.addEventListener('click', () => {
    fetch("http://127.0.0.1:54321/message/", {
        headers: {
            Accept: 'application/json' /* формат данных json */,
            'Content-Type': 'application/json' /* формат данных json */,
        },
        method: 'GET' /* метод */,
    }).then((res) =>
        res.json(),
    ) /* получаю ответ от сервера и отсеиваю лишнее чтобы оставить только данные в виде json */
        .then(result => {
            if (document.querySelector('.bodypoint.message') == null) {
                const he = document.createElement('h5')
                he.classList.add('headpoint', 'marg', 'mes')
                he.innerHTML = 'RESULT'
                const bodypoint = document.createElement('pre')
                bodypoint.classList.add('bodypoint', 'message')
                bodypoint.innerHTML = JSON.stringify(result, undefined, 2);
                point.append(he, bodypoint)
                point.classList.add('active')
            }
            else {
                const bodypoint = document.querySelector('.bodypoint.message')
                bodypoint.innerHTML = JSON.stringify(result, undefined, 2);
            }
        })
})