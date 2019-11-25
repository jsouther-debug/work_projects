import psycopg2
# Import a bunch of content from the Graphene library. These imports allow us
# to implement the GraphQL schema.
from graphene import ObjectType, String, Schema, Int, NonNull, Float, List, Field, DateTime, Mutation, InputObjectType
from psycopg2.extras import RealDictCursor

# Create a connection to Postgres. You will have to update the configuration information here.
db_connection = psycopg2.connect(
    dbname="jared_souther",
    user="jared_souther",
    password="passwd",
    host="faraday.cse.taylor.edu"
)

db_cursor = db_connection.cursor(
    cursor_factory=RealDictCursor
)

class Surface(ObjectType):
    id = Int(required=True)
    state = String(required=True)

class State(ObjectType):
    abbrev = String(required=True)
    state = String(required=True)

class RiderInput(InputObjectType):
    firstName = String(required=True)
    lastName = String(required=True)
    email = String(required=True)
    birthday = DateTime(required=True)

class TeamInput(InputObjectType):
    webSite = String
    contactEmail = String
    since = DateTime(required=True)

class Trail(ObjectType):
    id = Int(required=True)
    name = String(required=True)
    surface = Surface
    state = State
    latitude = Float(required=True)
    longitude = Float(required=True)

    def resolve_surface(trail,info):
        db_cursor.execute(
            """
            SELECT surface.id, firstName, FROM surface
            INNER JOIN trail 
              ON trail.surfaceID = surface.id
            WHERE surface.id = %(trail_pk)s 
            """,
            {"trail_pk": trail['trail_id']}
        )
        return [Surface(**row_data) for row_data in db_cursor.fetchall()]

    def resolve_state(trail,info):
        db_cursor.execute(
            """
            SELECT state.abbrev, name FROM state
            INNER JOIN trail 
              ON trail.stateAbbrev = state.abbrev
            WHERE state.abbrev = %(trail_pk)s 
            """,
            {"trail_pk": trail['trail_id']}
        )
        return [State(**row_data) for row_data in db_cursor.fetchall()]

class TrailInput(InputObjectType):
    name = String(required=True)
    surfaceId = Int(required=True)
    stateAbbrev = String(required=True)
    latitude = Float(required=True)
    longitude = Float(required=True)

class Ride(ObjectType):
    rider = lambda: Rider
    trail = Trail
    date = DateTime(required=True)
    disctance = Float(required=True)

    def resolve_rider(ride,info):
        db_cursor.execute(
            """
            SELECT rider.id, firstName, lastName, email, birthday FROM rider
            INNER JOIN ride 
              ON ride.cyclistId = rider.id
            WHERE rider.id = %(ride_pk)s 
            """,
            {"ride_pk": ride['ride_id']}
        )
        return [Rider(**row_data) for row_data in db_cursor.fetchall()]

    def resolve_trail(ride,info):
        db_cursor.execute(
            """
            SELECT trail.id, name, surfaceId, stateAbbrev, latitude, longitude FROM trail
            INNER JOIN ride 
              ON ride.trailId = trail.id
            WHERE trail.id = %(ride_pk)s 
            """,
            {"ride_pk": ride['ride_id']}
        )
        return [Trail(**row_data) for row_data in db_cursor.fetchall()]

class Team(ObjectType):
    id = Int(required=True)
    webSite = String()
    contactEmail = String()
    since = DateTime(required=True)
    riders = List(NonNull(lambda: Rider),required=True)

    def resolve_rides(team,info):
        db_cursor.execute(
            """
            SELECT rider.id, firstName, lastName, email, birthday FROM rider
            INNER JOIN teamMember tm
              ON rider.id = tm.cyclistId
            WHERE tm.cyclistId = %(team_pk)s 
            """,
            {"team_pk": team['team_id']}
        )
        return [Rider(**row_data) for row_data in db_cursor.fetchall()]

class Rider(ObjectType):
    id = Int(required=True)
    firstName = String(required=True)
    lastName = String(required=True)
    email = String(required=True)
    birthday = DateTime(required=True)
    teams = List(NonNull(Team),required=True)
    rides = List(NonNull(Ride),required=True)

    def resolve_teams(rider,info):
        db_cursor.execute(
            """
            SELECT team.id, website, contactEmail, since FROM team
            INNER JOIN teamMember tm
              ON team.id = tm.teamId
            WHERE tm.cyclistId = %(rider_pk)s 
            """,
            {"rider_pk": rider['rider_id']}
        )
        return [Rider(**row_data) for row_data in db_cursor.fetchall()]

    def resolve_rides(rider,info):
        db_cursor.execute(
            """
            SELECT ride.cyclistId, trailId, date, distance FROM ride
            INNER JOIN rider 
              ON rider.id = ride.cyclistId
            WHERE ride.cyclistId = %(rider_pk)s 
            """,
            {"rider_pk": rider['rider_id']}
        )
        return [Rider(**row_data) for row_data in db_cursor.fetchall()]

class RideInput(InputObjectType):
    riderId = Int(required=True)
    trailId = Int(required=True)
    date = DateTime(required=True)
    distance = Float(required=True)

class CreateRider(Mutation):
    """Create an rider."""
    class Arguments:
        rider_input = RiderInput(required=True)

    class Meta:
        # We're going to return not a `CreateRider`, but an `Rider`
        output = Rider

    def mutate(root, info, rider_input):
        db_cursor.execute(
            """
            INSERT INTO rider(firstName, lastName, email, birthday)
            VALUES (%(first)s, %(last)s, %(email)s, %(birth)s)
            RETURNING *
            """,
            {
                "first": rider_input.firstName,
                "last": rider_input.lastName,
                "email": rider_input.email,
                "birth": rider_input.birthday
            })
        new_rider = db_cursor.fetchone()
        db_connection.commit()
        return Rider(**new_rider)


class CreateTeam(Mutation):
    """Create an team."""
    class Arguments:
        team_input = TeamInput(required=True)

    class Meta:
        # We're going to return not a `CreateTeam`, but an `Team`
        output = Team

    def mutate(root, info, team_input):
        db_cursor.execute(
            """
            INSERT INTO team(webSite, contactEmail, since)
            VALUES (%(web)s, %(conemail)s, %(sin)s)
            RETURNING *
            """,
            {
                "web": rider_input.webSite,
                "conemail": rider_input.contactEmail,
                "sin": rider_input.since
            })
        new_team = db_cursor.fetchone()
        db_connection.commit()
        return Team(**new_team)

class CreateTrail(Mutation):
    """Create an Trail."""
    class Arguments:
        trail_input = TrailInput(required=True)

    class Meta:
        # We're going to return not a `CreateTrail`, but an `Team`
        output = Trail

    def mutate(root, info, trail_input):
        db_cursor.execute(
            """
            INSERT INTO trail(name, surfaceId, stateAbbrev, latitude, longitude)
            VALUES (%(n)s, %(sur)s, %(sa)s, %(la)s, %(lo)s)
            RETURNING *
            """,
            {
                "n": rider_input.name,
                "sur": rider_input.surfaceId,
                "sa": rider_input.stateAbbrev,
                "la": rider_input.latitude,
                "lo": rider_input.longitude
            })
        new_trail = db_cursor.fetchone()
        db_connection.commit()
        return Trail(**new_trail)

class logRide(Mutation):
    """log a Ride."""
    class Arguments:
        ride_input = RideInput(required=True)

    class Meta:
        # We're going to return not a `CreateTrail`, but an `Ride`
        output = Ride

    def mutate(root, info, ride_input):
        db_cursor.execute(
            """
            INSERT INTO ride(cyclistId, trailId, date, distance)
            VALUES (%(cId)s, %(tId)s, %(d)s, %(dis)s)
            RETURNING *
            """,
            {
                "cId": rider_input.cyclistId,
                "tId": rider_input.trailId,
                "d": rider_input.date,
                "dis": rider_input.distance
            })
        new_ride = db_cursor.fetchone()
        db_connection.commit()
        return Ride(**new_ride)

class Query(ObjectType):
    rider = Field(Rider, id=Int(required=True))
    riders = List(Rider, description="All the riders")
    team = Field(Team, id=Int(required=True))
    teams = List(Team, description="All the teams")
    trail = Field(Trail, id=Int(required=True))
    trails = List(Trail, description="All the trails")

    def resolve_rider(root, info, id):
        db_cursor.execute(
            """
            SELECT * FROM rider
            WHERE id = %(id)s
            """,
            {'id': id}
        )
        return Rider(**db_cursor.fetchone())

    # Resolve the list of all Riders.
    def resolve_riders(root, info):
        db_cursor.execute("SELECT * FROM rider")
        return db_cursor.fetchall()

    def resolve_team(root, info, id):
        db_cursor.execute(
            """
            SELECT * FROM team
            WHERE id = %(id)s
            """,
            {'id': id}
        )
        return Team(**db_cursor.fetchone())

    # Resolve the list of all teams.
    def resolve_teams(root, info):
        db_cursor.execute("SELECT * FROM team")
        return db_cursor.fetchall()


    def resolve_trail(root, info, id):
        db_cursor.execute(
            """
            SELECT * FROM trail
            WHERE id = %(id)s
            """,
            {'id': id}
        )
        return Trail(**db_cursor.fetchone())

    # Resolve the list of all the trails.
    def resolve_trails(root, info):
        db_cursor.execute("SELECT * FROM trail")
        return db_cursor.fetchall()


class Mutation(ObjectType):
    create_Rider = CreateRider.Field()
    create_Team = CreateTeam.Field()
    create_Trail = CreateTrail.Field()
    log_Ride = logRide.Field()

schema = Schema(
    query=Query,
    mutation=Mutation
)