const getbuttonmessage = document.querySelector('.getpointbutton.message')
const point = document.querySelector('.point.message')
const openbutton = document.querySelector('.openbutton.marg.message')
const closebutton = document.querySelector('.closebuton.marg.message')

getbuttonmessage.addEventListener('click', () => {
    fetch("http://127.0.0.1:43243/message/", {
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

openbutton.addEventListener('click', () => {
    if (document.querySelector('.bodypoint.preview') == null) {
        const he = document.createElement('h5')
        he.classList.add('headpoint', 'marg', 'mes')
        he.innerHTML = 'PREVIEW'
        const bodypoint = document.createElement('pre')
        bodypoint.classList.add('bodypoint', 'preview')
        bodypoint.innerHTML = JSON.stringify(JSON.parse('{"message": "text","username": "text"}'), undefined, 2)
        point.append(he, bodypoint)
        point.classList.add('active')
    }
    point.classList.add('active')
})

closebutton.addEventListener('click', () => {
    if (document.querySelector('.bodypoint.preview') !== null) {
        point.removeChild(point.querySelector('.headpoint.marg.mes'))
        point.removeChild(point.querySelector('.bodypoint.preview'))
        point.classList.remove('active')
    }
    if (document.querySelector('.bodypoint.message') !== null) {
        point.removeChild(point.querySelector('.headpoint.marg.mes'))
        point.removeChild(point.querySelector('.bodypoint.message'))
        point.classList.remove('active')
    }
})

fetch("http://127.0.0.1:54321/message/", {
    headers: {
        Accept: 'application/json' /* формат данных json */,
        'Content-Type': 'application/json' /* формат данных json */,
    },
    method: 'POST' /* метод */,
    body: "{\"user\":\"tola\"}"
}).then((res) =>
    res.json(),
) /* получаю ответ от сервера и отсеиваю лишнее чтобы оставить только данные в виде json */
    .then(result => {
        console.log(result)
    })