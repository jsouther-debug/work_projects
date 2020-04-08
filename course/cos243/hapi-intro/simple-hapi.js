const knex = require('knex')({
    client: 'pg',
    connection: {
        host: 'fake_host_url',
        user: 'fake_user',
        password: 'fake_password',
        database: 'fake_db'
    }
});

async function get_countries(){
    return await knex('country')
    .select('country_id', 'country')
}

async function get_film(id){
    return await knex('film')
    .select('film_id', 'title', 'rental_rate')
    .where('film_id', id)
    .first()
}

async function get_films(key){
    if (!key) {
        return await knex('film')
        .select('film_id', 'title', 'rental_rate')
    }
    else {
        return await knex('film')
        .select('film_id', 'title', 'rental_rate')
        .where('title', 'like', key)
    }
}

const Hapi = require('@hapi/hapi');

const init = async () => {
    const server = Hapi.server({
        host: 'localhost',
        port: 3000
    });

    await server.register({plugin: require('blipp'),
                            options: {showAuth: true}});

    
    await server.register({
        plugin: require('hapi-pino'),
        options: {
            prettyPrint: true
        }
    });

    server.route([
        {
            method: 'GET',
            path: '/',
            handler: (request, h) => {
                return 'Hello, Hapi';
            }
        },
        {
            method: 'GET',
            path: '/countries',
            handler: (request, h) => {
                return get_countries();
            }
        },
        {
            method: 'GET',
            path: '/films/{id}',
            handler: (request, h) => {
                film_id = request.params.id;
                return get_film(film_id);
            }
        },
        {
            method: 'GET',
            path: '/films',
            handler: (request, h) => {
                const searchkey = request.query.title;
                return get_films(searchkey);
            }
        },
    ]);

    await server.start();
}

process.on('unhandledRejection', err => {
    console.error(err);
    process.exit(1);
});

init();
