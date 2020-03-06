   
objection = require('objection');
const Model = objection.Model;
Model.knex(knex);

class Country extends Model {
    static get tableName() {
        return 'country';
    }
}

Country.query()
    .then(countries => {
        countries.forEach(country => {
        if (country.country.startsWith('F')) {
            console.log(country.country_id,
            country.country);
        }
    });
  //  knex.destroy();
    })
    .catch(err => console.log(err.message));


class Actor extends Model {
    static get tableName() {
        return 'actor';
    }
}

function actors_starting_with(prefix) {
    Actor.query()
        .select('first_name', 'last_name')
        .where('first_name', 'like', `${prefix}%`)
        .orWhere('last_name', 'like', `${prefix}%`)
        .then(actors => {
            actors.forEach(actor => {
                console.log(`${actor.first_name} ${actor.last_name}`);
            })
            knex.destroy();
        })
        .catch(err => console.log(err));        
}


actors_starting_with('F');