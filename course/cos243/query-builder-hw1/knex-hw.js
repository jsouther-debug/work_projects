const knex = require("knex")({
    client: 'pg',
    connection: {
        host: 'faraday.cse.taylor.edu',
        user: 'readonly',
        password: 'nerds4christ',
        database: 'dvdrental'
    }
});

knex('category')
 .select('category_id', 'name')
 .then(result => console.log(result))
 .then(() => knex.destroy());

 knex('film')
 .select('title', 'rental_duration')
 .where('title', like, 'S%')
 .andWhereBetween('rental_duration', [4,6])
 .orderBy([{column: 'rental_duration', order: 'desc'}, {column: 'title'}])
 .then(result => console.log(result))
 .then(() => knex.destroy());

 knex()
 .select('country')
 .count('city_id', {as: 'city_count'})
 .from('city')
 .innerJoin('country', 'city.country_id', 'country.country_id')
 .groupBy('country')
 .orderBy([{column: 'city_count', order: 'desc'}])
 .then(result => console.log(result))
 .then(() => knex.destroy());