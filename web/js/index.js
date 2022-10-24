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
        console.log(result);
    })